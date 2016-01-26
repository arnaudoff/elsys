//--------------------------------------------
//// NAME: Ivaylo Arnaudov
//// CLASS: XIb
//// NUMBER: 13
//// PROBLEM: #2
//// FILE NAME: shell.c
//// FILE PURPOSE:
//// Implements a simple shell that is capable of
//// executing commands as child processes.
////---------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

//--------------------------------------------
//// FUNCTION: parse_cmdline
////   Takes a command line, parses it and returns a pointer to an array
////   of pointers to null-terminated character strings with a sentinel value
////   of NULL so that the result of the function can be directly passed to execv.
//// PARAMETERS:
////   cmdline - the shell command to be parsed
////----------------------------------------------
char** parse_cmdline(const char *cmdline) {
    size_t cmdline_len = strlen(cmdline);
    // In order to keep the function prototype and have it work with strok
    char *buf_cmd_line = (char*)malloc(cmdline_len);
    if (buf_cmd_line == NULL) {
        fprintf(stderr, "Could not allocate memory for command line buffer");
        return NULL;
    }

    // Copy to the buffer while removing the newline byte
    strncpy(buf_cmd_line, cmdline, cmdline_len - 1);
    buf_cmd_line[cmdline_len - 1] = '\0';

    char **args = (char**)malloc(sizeof(char*));
    if (args == NULL) {
        fprintf(stderr, "Could not allocate memory for command line args");
        return NULL;
    }

    char *current_arg = strtok(buf_cmd_line, " ");
    args[0] = current_arg;
    int args_count = 2;
    while ((current_arg = strtok(NULL, " ")) != NULL) {
        args = (char**)realloc(args, args_count * sizeof(char*));
        if (args == NULL) {
            fprintf(stderr, "Could not realloc for command line args.");
            return NULL;
        }

        args[args_count - 1] = current_arg;
        args_count++;
    }

    // Since execv requires a NULL pointer which marks the end of the array.
    args = (char**)realloc(args, ((args_count - 1) * sizeof(char*)) + sizeof(NULL));
    if (args == NULL) {
        fprintf(stderr, "Could not realloc for args' sentinel value.");
        return NULL;
    }
    args[args_count - 1] = NULL;

    return args;
}

//--------------------------------------------
//// FUNCTION: execute
////   Does the actual work on executing commands as child processes.
//// PARAMETERS:
////   filename - the name of the executable
////   args - pointer to an array of pointers to null-terminated char arrays
////   with sentinel value of NULL; it's passed directly to execv.
////----------------------------------------------
int execute(const char *filename, char **args) {
    pid_t pid = fork();

    if (pid >= 0) {
        if (pid == 0) { // Child process
            int exec_res = execv(filename, args);
            if (exec_res == -1) {
                perror(filename);
                exit(EXIT_FAILURE);
            } else {
                exit(EXIT_SUCCESS);
            }
        } else { // Parent process
            int child_status;
            pid_t child_pid = waitpid(pid, &child_status, 0);
            if (child_pid == -1) {
                fprintf(stderr, "Could not retrieve child's status");
                return -1;
            }
            return WEXITSTATUS(child_status);
        }
    } else {
        fprintf(stderr, "Could not fork in order to execute %s", filename);
        return -1;
    }

    return 0;
}

int main() {
    while (1) {
        char *buffer = NULL;
        size_t len;
        int read = getline(&buffer, &len, stdin);
        if (read == -1) {
            return 1;
        } else if (strcmp(buffer, "exit\n") == 0) {
            return 0;
        } else {
            char** command_tokens = parse_cmdline(buffer);
            if (command_tokens == NULL) {
                return 1;
            }

            execute(command_tokens[0], command_tokens);
            free(command_tokens);
        }

        if (buffer != NULL) {
            free(buffer);
        }
    }

    return 0;
}
