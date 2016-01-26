//--------------------------------------------
//// NAME: Ivaylo Arnaudov
//// CLASS: XIb
//// NUMBER: 13
//// PROBLEM: #1
//// FILE NAME: tail.c
//// FILE PURPOSE:
//// Implementation of the Unix tail utility. It takes the last
//// ten lines from a file or from stdin and displays them. Useful for logs.
////---------------------------------------------

#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define ERROR_BUFF_SIZE 256
#define FILENAME_MESSAGE_BUFF_SIZE 300
#define INITIAL_STDIN_INPUT_SIZE 512
#define STDIN_READ_BUFFER_SIZE 512

//--------------------------------------------
//// FUNCTION: print_file_error
//// Writes the last error that occured while tailing a file to stderr.
//// PARAMETERS:
//// filename - the name of the file on which the error occured
//// message - message for the error, ie. strerror(errno)
////----------------------------------------------
void print_file_error(const char *filename, char *message) {
    char buf[ERROR_BUFF_SIZE];
    int res = snprintf(buf, ERROR_BUFF_SIZE,
            "tail: cannot open '%s' for reading: %s\n", filename, message);
    write(STDERR_FILENO, buf, res);
}

//--------------------------------------------
//// FUNCTION: print_stdin_error
//// Writes the last error that occured while tailing from stdin.
//// PARAMETERS:
//// message - message for the error, ie. strerror(errno)
////----------------------------------------------
void print_stdin_error(const char *message) {
    char buf[ERROR_BUFF_SIZE];
    int res = snprintf(buf, ERROR_BUFF_SIZE,
            "tail: cannot read from stdin: %s\n", message);
    write(STDERR_FILENO, buf, res);
}

//--------------------------------------------
//// FUNCTION: print_filename
//// Prints the name of the file the tail of which is being shown.
//// PARAMETERS:
//// filename - the name of the file
////----------------------------------------------
void print_filename(const char *filename) {
    // ext2, ext3, ext4 and ntfs limit file names to 255 chars
    char message_buf[FILENAME_MESSAGE_BUFF_SIZE];
    int res = snprintf(message_buf, FILENAME_MESSAGE_BUFF_SIZE,
            "==> %s <==\n", filename);

    write(STDOUT_FILENO, message_buf, res);
}

//--------------------------------------------
//// FUNCTION: get_stdin_tail
//// Reads from stdin and gets the last 10 lines, printing them respectively.
////----------------------------------------------
int get_stdin_tail() {
    char *ten_lines_chunk = (char*)malloc(INITIAL_STDIN_INPUT_SIZE * sizeof(char) + 1);
    if (ten_lines_chunk == NULL) {
        print_stdin_error("Could not allocate initial memory for stdin.");
        return -1;
    }

    int bytes_read = 0;
    int total_bytes_read = 0;

    int curr_tl_chunk_size = INITIAL_STDIN_INPUT_SIZE;
    char read_buf[STDIN_READ_BUFFER_SIZE];
    int new_chunk_size = 0;

    int linebreaks_count = 0;
    int linebreaks_indices[11];
    while ((bytes_read = read(STDIN_FILENO, read_buf, STDIN_READ_BUFFER_SIZE)) > 0) {
        if (bytes_read < 0) {
            print_stdin_error(strerror(errno));
            return -1;
        }

        if (new_chunk_size + bytes_read > curr_tl_chunk_size) {
            ten_lines_chunk = (char*)realloc(ten_lines_chunk, new_chunk_size + bytes_read + 1);
            if (ten_lines_chunk == NULL) {
                print_stdin_error("Failed to realloc the ten lines chunk");
                return -1;
            }

            curr_tl_chunk_size = new_chunk_size + bytes_read + 1;
        }

        strncat(ten_lines_chunk, read_buf, bytes_read);

        for (int i = 0; i < bytes_read; i++) {
            if (read_buf[i] == '\n') {
                linebreaks_indices[linebreaks_count++] = new_chunk_size + i;
            }

            if (linebreaks_count == 11) {
                new_chunk_size = (new_chunk_size + i + 1) - (linebreaks_indices[0] + 1);

                // Check whether resize of the ten lines chunk is needed
                if (new_chunk_size + 1 > curr_tl_chunk_size) {
                    ten_lines_chunk = (char*)realloc(ten_lines_chunk, new_chunk_size + 1);
                    if (ten_lines_chunk == NULL) {
                        print_stdin_error("Failed to realloc the ten lines chunk.");
                        return -1;
                    }
                    curr_tl_chunk_size = new_chunk_size + 1;
                }

                /*
                 * If the ten lines chunk is large enough or has been resized,
                 * allocate a sub buffer to store a "substring" starting from
                 * the index of the first newline character.
                 */

                char *subbuf = (char*)malloc(new_chunk_size + 1);
                memcpy(subbuf, &ten_lines_chunk[linebreaks_indices[0] + 1], new_chunk_size);
                subbuf[new_chunk_size] = '\0';

                // Copy the sub buffer to the ten lines chunk and deallocate it
                strcpy(ten_lines_chunk, subbuf);
                free(subbuf);

                // Shift the linebreaks array by one as the zeroth newline has
                // been "dropped" out
                for (int j = 0; j < 10; j++) {
                    linebreaks_indices[j] = linebreaks_indices[j + 1];
                }

                linebreaks_count--;
            }
        }

        total_bytes_read += bytes_read;
    }

    ten_lines_chunk[new_chunk_size] = '\0';

    for (int i = 0; i < new_chunk_size; i++) {
        write(STDOUT_FILENO, &ten_lines_chunk[i], 1);
    }

    free(ten_lines_chunk);
    return 0;
}

//--------------------------------------------
//// FUNCTION: get_file_tail
//// Gets the last 10 lines of a file.
//// PARAMETERS:
//// path - path to the file to get the last 10 lines of.
////----------------------------------------------
int get_file_tail(const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        print_file_error(path, strerror(errno));
        return -1;
    }

    // Go to end of file
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size < 0) {
        print_file_error(path, strerror(errno));
        return -1;
    }

    char buf[1024];
    int lseek_offset;
    if (file_size < 1024) {
        lseek_offset = file_size;
    } else {
        lseek_offset = 1024;
    }

    // Read the file backwards till 10 lines are reached or file ends
    int linebreaks_count = 0;
    int last_line_pos = 0;
    int bytes_left_to_read = file_size;
    while (bytes_left_to_read > 0 || linebreaks_count == 11) {
        lseek(fd, file_size - lseek_offset, SEEK_SET);
        int bytes_read = read(fd, buf, lseek_offset);
        if (bytes_read < 0) {
            print_file_error(path, strerror(errno));
            return -1;
        }

        buf[bytes_read] = '\0';
        for (int i = bytes_read - 1; i >= 0; i--) {
            if (buf[i] == '\n') {
                linebreaks_count++;
                // When the 11th \n is reached, the 10th line starts from its
                // index + 1
                if (linebreaks_count == 11) {
                    last_line_pos = i + 1;
                    break;
                }
            }
        }

        bytes_left_to_read -= bytes_read;
    }

    for (int i = last_line_pos; buf[i] != '\0'; i++) {
        write(STDOUT_FILENO, &buf[i], 1);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    // If no argument is supplied
    if (argc == 1) {
        if (get_stdin_tail() < 0) {
            exit(0);
        }
    } else {
        int has_multiple_args = (argc > 2) ? 1 : 0;

        // If only one argument is supplied
        if (!has_multiple_args) {
            if (strcmp(argv[1], "-") == 0) {
                if (get_stdin_tail() < 0) {
                    exit(0);
                }
            } else {
                if (get_file_tail(argv[1])) {
                    exit(0);
                }
            }
        } else {
            for (int i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-") == 0) {
                    print_filename("standard input");
                    if (get_stdin_tail() < 0) {
                        exit(0);
                    }
                } else {
                    print_filename(argv[i]);
                    if (get_file_tail(argv[i]) < 0) {
                        exit(0);
                    }
                }

                write(STDOUT_FILENO, "\n", 1);
            }
        }
    }

    return 0;
}
