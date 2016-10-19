//--------------------------------------------
//// NAME: Ivaylo Arnaudov
//// CLASS: XIb
//// NUMBER: 13
//// PROBLEM: #3
//// FILE NAME: starcraft3.c
//// FILE PURPOSE:
////    Implements a simple, multithreaded prototype of the StarCraft game.
////---------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define STARTING_WORKERS_COUNT 5
#define MAP_MINERALS_COUNT 5000
#define MINERALS_MINED_PER_WORKER 8
#define SOLDIER_TRAIN_COST 50
#define WORKER_TRAIN_COST 50
#define COMMAND_CENTER_COST 400
#define GAME_FINISH_SOLDIERS_COUNT 20
#define MAX_SOLDIERS_WORKERS 200

void init_mutex(pthread_mutex_t *mutex, const char *mutex_desc);

pthread_mutex_t *command_center_mutexes;
pthread_mutex_t mined_minerals_mutex;
pthread_mutex_t inventory_minerals_mutex;
pthread_mutex_t left_minerals_mutex;
pthread_mutex_t cc_count_mutex;
pthread_mutex_t current_workers_mutex;
pthread_mutex_t current_soldiers_mutex;

int mined_minerals = 0;
int inventory_minerals = 0;
int left_minerals = MAP_MINERALS_COUNT;
int current_command_centers_count = 1;
int current_soldiers_count = 0;
int current_workers_count = STARTING_WORKERS_COUNT;

pthread_t* workers;

//--------------------------------------------
//// FUNCTION: worker_finished
////    A thread safe function which checks whether a particular SCV has
////    finished working. Used as a condition in the worker thread.
////----------------------------------------------
int worker_finished() {
    pthread_mutex_lock(&current_soldiers_mutex);
    pthread_mutex_lock(&current_workers_mutex);
    if (current_soldiers_count < GAME_FINISH_SOLDIERS_COUNT &&
            current_soldiers_count + current_workers_count < MAX_SOLDIERS_WORKERS) {
        pthread_mutex_unlock(&current_soldiers_mutex);
        pthread_mutex_unlock(&current_workers_mutex);
        return 0;
    }

    pthread_mutex_unlock(&current_soldiers_mutex);
    pthread_mutex_unlock(&current_workers_mutex);

    return 1;
}

//--------------------------------------------
//// FUNCTION: mine
////    Defines the way an SCV (worker) behaves. The routine is passed as a
////    function pointer to a pthread_create call in order for SCVs to work
////    in parallel.
//// PARAMETERS:
////    arg - the ID of the SCV worker to start.
////----------------------------------------------
void *mine(void *arg) {
    int scv_id = *((int *)arg);

    while (!worker_finished()) {
        pthread_mutex_lock(&left_minerals_mutex);

        if (left_minerals >= MINERALS_MINED_PER_WORKER) {
            left_minerals -= MINERALS_MINED_PER_WORKER;
            printf("SCV %d is mining\n", scv_id);
        } else {
            pthread_mutex_unlock(&left_minerals_mutex);
            break;
        }

        pthread_mutex_unlock(&left_minerals_mutex);

        pthread_mutex_lock(&mined_minerals_mutex);
        mined_minerals += MINERALS_MINED_PER_WORKER;
        pthread_mutex_unlock(&mined_minerals_mutex);

        printf("SCV %d is transporting minerals\n", scv_id);
        sleep(2);

        pthread_mutex_lock(&mined_minerals_mutex);
        if (mined_minerals < MAP_MINERALS_COUNT) {
            pthread_mutex_unlock(&mined_minerals_mutex);

            int command_center_index = 0;
            while (pthread_mutex_trylock(&command_center_mutexes[command_center_index]) != 0) {
                command_center_index++;

                pthread_mutex_lock(&cc_count_mutex);
                if (command_center_index == current_command_centers_count) {
                    pthread_mutex_unlock(&cc_count_mutex);
                    command_center_index = 0;
                    sleep(1);
                } else {
                    pthread_mutex_unlock(&cc_count_mutex);
                }
            }

            pthread_mutex_lock(&inventory_minerals_mutex);
            inventory_minerals += MINERALS_MINED_PER_WORKER;
            pthread_mutex_unlock(&inventory_minerals_mutex);

            printf("SCV %d delivered minerals to Command Center %d\n",
                    scv_id, command_center_index + 1);
            pthread_mutex_unlock(&command_center_mutexes[command_center_index]);
        } else {
            pthread_mutex_unlock(&mined_minerals_mutex);
            break;
        }
    }

    free(arg);
    return NULL;
}

//--------------------------------------------
//// FUNCTION: train_soldier
////    Defines the way a soldier is trained. Also checks for logical errors
////    and takes care of maintaining the correct minerals state.
////----------------------------------------------
void train_soldier() {
    pthread_mutex_lock(&inventory_minerals_mutex);
    if (inventory_minerals < SOLDIER_TRAIN_COST) {
        pthread_mutex_unlock(&inventory_minerals_mutex);
        printf("Not enough minerals.\n");
        return;
    }

    inventory_minerals -= SOLDIER_TRAIN_COST;
    pthread_mutex_unlock(&inventory_minerals_mutex);

    sleep(1);

    pthread_mutex_lock(&current_soldiers_mutex);
    current_soldiers_count++;
    pthread_mutex_unlock(&current_soldiers_mutex);

    printf("You wanna piece of me, boy?\n");
}

//--------------------------------------------
//// FUNCTION: train_worker
////    Defines the way a worker is trained. Also checks for logical errors
////    and takes care of maintaining the correct minerals state.
////    Internally, once the time simulation is finished, creates a new thread
////    for the SCV and adds it's pthread_t to the global array of workers.
////----------------------------------------------
void train_worker() {
    pthread_mutex_lock(&inventory_minerals_mutex);
    if (inventory_minerals < WORKER_TRAIN_COST) {
        pthread_mutex_unlock(&inventory_minerals_mutex);
        printf("Not enough minerals.\n");
        return;
    }

    inventory_minerals -= WORKER_TRAIN_COST;
    pthread_mutex_unlock(&inventory_minerals_mutex);

    sleep(1);

    pthread_mutex_lock(&current_workers_mutex);
    current_workers_count++;
    pthread_mutex_unlock(&current_workers_mutex);

    workers = (pthread_t*)realloc(workers, sizeof(pthread_t) *
            current_workers_count);
    if (workers == NULL) {
        fprintf(stderr, "realloc could not reallocate memory for the workers\n");
        exit(-1);
    }

    int *new_scv_id = (int *)malloc(sizeof(*new_scv_id));
    if (new_scv_id == NULL) {
        fprintf(stderr, "malloc failed to allocate memory for new worker thread arg\n");
        exit(-1);
    }

    *new_scv_id = current_workers_count;

    if (pthread_create(&workers[current_workers_count - 1], NULL, mine, new_scv_id) != 0) {
        fprintf(stderr, "pthread_create failed for new worker thread\n");
        exit(-1);
    }

    printf("SCV good to go, sir.\n");
}

//--------------------------------------------
//// FUNCTION: build_command_center
////    Defines the way a command center is build. Checks for logical
////    errors, takes care of maintaining the correct minerals state and
////    finally initializes a mutex for the newly built center.
////----------------------------------------------
void build_command_center() {
    pthread_mutex_lock(&inventory_minerals_mutex);
    if (inventory_minerals < COMMAND_CENTER_COST) {
        pthread_mutex_unlock(&inventory_minerals_mutex);
        printf("Not enough minerals.\n");
        return;
    }

    inventory_minerals -= COMMAND_CENTER_COST;
    pthread_mutex_unlock(&inventory_minerals_mutex);

    sleep(2);

    pthread_mutex_lock(&cc_count_mutex);
    current_command_centers_count++;
    pthread_mutex_unlock(&cc_count_mutex);

    command_center_mutexes = (pthread_mutex_t *)realloc(command_center_mutexes,
            current_command_centers_count * sizeof(pthread_mutex_t));

    init_mutex(&command_center_mutexes[current_command_centers_count - 1],
            "new command center");

    printf("Command center %d created.\n", current_command_centers_count);
}

//--------------------------------------------
//// FUNCTION: init_mutex
////    A wrapper function for initializing a mutex that does error handling.
//// PARAMETERS:
////    mutex - the address of the mutex to initialize
////    mutex_desc - description of the mutex, for easier debugging
////----------------------------------------------
void init_mutex(pthread_mutex_t *mutex, const char *mutex_desc) {
    int rc = pthread_mutex_init(mutex, NULL);
    if (rc != 0) {
        fprintf(stderr,
                "pthread_mutex_init failed for %s: %s\n",
                mutex_desc,
                strerror(rc));
        exit(-1);
    }
}

//--------------------------------------------
//// FUNCTION: destroy_mutex
////    A wrapper function for destroying a mutex that does error handling.
//// PARAMETERS:
////    mutex - the address of the mutex to destroy
////    mutex_desc - description of the mutex, for easier debugging
////----------------------------------------------
void destroy_mutex(pthread_mutex_t *mutex, const char *mutex_desc) {
    int rc = pthread_mutex_destroy(mutex);
    if (rc != 0) {
        fprintf(stderr,
                "pthread_mutex_destroy failed for %s: %s\n",
                mutex_desc,
                strerror(rc));
        exit(-1);
    }
}

//--------------------------------------------
//// FUNCTION: input_handler
////    A function that is used as an UI thread, responsible for handling
////    the user input.
//// PARAMETERS:
////    arg - not used; has to be there by convention
////----------------------------------------------
void *input_handler(void *arg) {
    char c;
    while (current_soldiers_count < 20) {
        c = getchar();
        if (c == 'm') {
            train_soldier();
        } else if (c == 's') {
            train_worker();
        } else if (c == 'c') {
            build_command_center();
        }
    }

    return NULL;
}

//--------------------------------------------
//// FUNCTION: main
////    The entry point; creates the SCV threads and the UI thread, initializes
////    the required mutexes for synchronization. Finally joins the threads and
////    frees the used resources.
//// PARAMETERS:
////    arg - not used; has to be there by convention
////----------------------------------------------
int main() {
    init_mutex(&mined_minerals_mutex, "mined minerals count");
    init_mutex(&inventory_minerals_mutex, "inventory minerals count");
    init_mutex(&left_minerals_mutex, "left minerals count");

    command_center_mutexes = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    if (command_center_mutexes == NULL) {
        fprintf(stderr, "failed to allocate memory for the command center mutexes\n");
        return -1;
    }

    init_mutex(&command_center_mutexes[0], "first command center");
    init_mutex(&cc_count_mutex, "command centers count");
    init_mutex(&current_workers_mutex, "current workers count");
    init_mutex(&current_soldiers_mutex, "current soldiers count");

    workers = (pthread_t*)malloc(sizeof(pthread_t) * STARTING_WORKERS_COUNT);
    if (workers == NULL) {
        fprintf(stderr, "failed to allocate memory for the initial worker threads\n");
        return -1;
    }

    pthread_t input_handler_thread;
    if (pthread_create(&input_handler_thread, NULL, input_handler, NULL) != 0) {
        fprintf(stderr, "failed to spawn UI thread");
        return -1;
    }

    for (int i = 0; i < STARTING_WORKERS_COUNT; i++) {
        int *scv_id = (int*)malloc(sizeof(*scv_id));
        if (scv_id == NULL) {
            fprintf(stderr, "malloc failed to allocate memory for worker thread arg\n");
            return -1;
        }

        *scv_id = i + 1;
        if (pthread_create(&workers[i], NULL, mine, scv_id) != 0) {
            fprintf(stderr, "pthread_create failed for one of the initial workers\n");
            return -1;
        }
    }

    if (pthread_join(input_handler_thread, NULL) != 0) {
        fprintf(stderr, "pthread_join failed to join UI thread");
        return -1;
    }

    for (int i = 0; i < current_workers_count; i++) {
        if (pthread_join(workers[i], NULL) != 0) {
            fprintf(stderr, "pthread_join failed to join one of the worker threads");
            return -1;
        }
    }

    destroy_mutex(&mined_minerals_mutex, "mined minerals count");
    destroy_mutex(&inventory_minerals_mutex, "inventory minerals count");
    destroy_mutex(&left_minerals_mutex, "left minerals count");
    destroy_mutex(&cc_count_mutex, "command centers count");
    destroy_mutex(&current_workers_mutex, "current workers count");
    destroy_mutex(&current_soldiers_mutex, "current soldiers count");

    for (int i = 0; i < current_command_centers_count; i++) {
        destroy_mutex(&command_center_mutexes[i], "one of the command centers");
    }

    free(workers);
    free(command_center_mutexes);

    printf("Starting: %d Left: %d Total mined: %d\n",
            MAP_MINERALS_COUNT, left_minerals, mined_minerals);

    return 0;
}
