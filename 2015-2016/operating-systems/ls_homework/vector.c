//--------------------------------------------
// NAME: Ivaylo Arnaudov
// CLASS: XIb
// NUMBER: 13
// PROBLEM: #2
// FILE NAME: vector.c
// FILE PURPOSE: Implements a simple vector to hold stat structs and paths.
//---------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "vector.h"

//--------------------------------------------
//// FUNCTION: vector_init
////    Initializes a vector with size, capacity and allocates an array of
////    stat structs and array of strings for the paths.
//// PARAMETERS:
////    vector - pointer to a file_stat_vector_t
////----------------------------------------------
void vector_init(file_stat_vector_t *vector) {
    vector->size = 0;
    vector->capacity = VECTOR_INITIAL_CAPACITY;
    vector->stats = (struct stat*)malloc(sizeof(struct stat) * vector->capacity);
    if (vector->stats == NULL) {
        fprintf(stderr, "Could not allocate memory for file stat vector");
        exit(EXIT_FAILURE);
    }

    vector->paths = (char**)malloc(sizeof(char*) * vector->capacity);
    if (vector->paths == NULL) {
        fprintf(stderr, "Could not allocate memory for file stat vector paths");
        exit(EXIT_FAILURE);
    }
}

//--------------------------------------------
//// FUNCTION: vector_push
////    Adds a new element to the vector.
//// PARAMETERS:
////    vector - pointer to a file_stat_vector_t
////    file_stat - a stat (sys/stat.h) struct
////    file_path - the path of the file that's been stat'd
////----------------------------------------------
void vector_push(file_stat_vector_t *vector, struct stat file_stat, char *file_path) {
    vector_resize(vector);
    vector->stats[vector->size] = file_stat;
    vector->paths[vector->size++] = file_path;
}

//--------------------------------------------
//// FUNCTION: vector_get
////    Returns the i-th element of the vector as a file_stat_vector_entry_t.
//// PARAMETERS:
////    vector - pointer to a file_stat_vector_t
////    index - the index of the vector element to retrieve
////----------------------------------------------
file_stat_vector_entry_t vector_get(file_stat_vector_t *vector, int index) {
    file_stat_vector_entry_t entry;

    entry.stat = vector->stats[index];
    entry.path = vector->paths[index];

    return entry;
}

//--------------------------------------------
//// FUNCTION: vector_resize
////    Doubles the capacity of the vector if it is full.
//// PARAMETERS:
////    vector - pointer to a file_stat_vector_t
////----------------------------------------------
void vector_resize(file_stat_vector_t *vector) {
    if (vector->size >= vector->capacity) {
        vector->capacity *= 2;
        vector->stats = (struct stat *)realloc(vector->stats, sizeof(struct stat) *
                vector->capacity);
        if (vector->stats == NULL) {
            fprintf(stderr, "Could not reallocate memory for file stat vector");
            exit(EXIT_FAILURE);
        }

        vector->paths = (char **)realloc(vector->paths, sizeof(char*) *
                vector->capacity);
        if (vector->paths == NULL) {
            fprintf(stderr, "Could not reallocate memory for file stat vector paths");
            exit(EXIT_FAILURE);
        }
    }
}

//--------------------------------------------
//// FUNCTION: vector_free
////    Frees up the space allocated on the heap for the vector, including the
////    elements of the char** array of paths.
//// PARAMETERS:
////    vector - pointer to a file_stat_vector_t
////----------------------------------------------
void vector_free(file_stat_vector_t *vector) {
    free(vector->stats);

    for (int i = 0; i < vector->size; i++) {
        free(vector->paths[i]);
    }

    free(vector->paths);
}
