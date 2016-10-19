//--------------------------------------------
// NAME: Ivaylo Arnaudov
// CLASS: XIb
// NUMBER: 13
// PROBLEM: #2
// FILE NAME: ls.c
// FILE PURPOSE: Implements the main logic for the `ls` command.
//---------------------------------------------

#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "vector.h"

#define TIME_BUFFER_SIZE 80

typedef struct {
    nlink_t largest_hardlinks_count;
    off_t largest_file_size;
    size_t longest_username;
    size_t longest_groupname;
} indent_info_t;

void list_file(const char *path, struct stat file_stat, int detailed, indent_info_t indent_info);
void list_directory(const char *dir_path, int multiple, int detailed, int show_hidden, int recursive);

char get_filetype_letter(int mode);
unsigned count_digits(unsigned long long value);
char* get_repeated_spaces(unsigned count);
unsigned long long max(unsigned long long a, unsigned long long b);
char* get_username(struct stat file_stat);
char* get_groupname(struct stat file_stat);

//--------------------------------------------
//// FUNCTION: list_file
////    Prints a single line of information about a certain file.
//// PARAMETERS:
////    path - path to the file
////    file_stat - a stat structure (sys/stat.h) returned from a call to stat
////    detailed - zero for only type and name, non-zero for details (ls -l)
////    indent_info - a indent_info_t structure, for padding left with spaces
////----------------------------------------------
void list_file(const char *path, struct stat file_stat, int detailed, indent_info_t indent_info) {
    char *dir = strdup(path);
    char *file_name = basename(dir);

    if (detailed) {
        char *user_name = get_username(file_stat);
        char *group_name = get_groupname(file_stat);

        unsigned long long hardlinks_count = file_stat.st_nlink;
        unsigned long long file_size = file_stat.st_size;

        unsigned hl_indent_len = count_digits(indent_info.largest_hardlinks_count) - count_digits(hardlinks_count);
        unsigned user_indent_len = indent_info.longest_username - strlen(user_name);
        unsigned group_indent_len = indent_info.longest_groupname - strlen(group_name);
        unsigned fs_indent_len = count_digits(indent_info.largest_file_size) - count_digits(file_size);

        char* hl_indent = get_repeated_spaces(hl_indent_len);
        char* user_indent = get_repeated_spaces(user_indent_len);
        char* group_indent = get_repeated_spaces(group_indent_len);
        char* fs_indent = get_repeated_spaces(fs_indent_len);

        char time_buffer[TIME_BUFFER_SIZE];
        struct tm *modified;
        if ((modified = localtime(&file_stat.st_mtime)) == NULL) {
            perror("localtime");
            exit(EXIT_FAILURE);
        }
        strftime(time_buffer, TIME_BUFFER_SIZE, "%b %e %R", modified);

        printf("%c%c%c%c%c%c%c%c%c%c %s%llu %s%s %s%s %s%llu %s %s\n",
                get_filetype_letter(file_stat.st_mode),
                (file_stat.st_mode & S_IRUSR) ? 'r' : '-',
                (file_stat.st_mode & S_IWUSR) ? 'w' : '-',
                (file_stat.st_mode & S_IXUSR) ? 'x' : '-',
                (file_stat.st_mode & S_IRGRP) ? 'r' : '-',
                (file_stat.st_mode & S_IWGRP) ? 'w' : '-',
                (file_stat.st_mode & S_IXGRP) ? 'x' : '-',
                (file_stat.st_mode & S_IROTH) ? 'r' : '-',
                (file_stat.st_mode & S_IWOTH) ? 'w' : '-',
                (file_stat.st_mode & S_IXOTH) ? 'x' : '-',
                hl_indent,
                hardlinks_count,
                user_name,
                user_indent,
                group_name,
                group_indent,
                fs_indent,
                file_size,
                time_buffer,
                file_name);

        free(hl_indent);
        free(user_indent);
        free(group_indent);
        free(fs_indent);
    } else {
        printf("%c %s\n", get_filetype_letter(file_stat.st_mode), file_name);
    }
}

//--------------------------------------------
//// FUNCTION: list_directory
////    Prints information about files in a certain directory.
//// PARAMETERS:
////    path - path to the directory
////    multiple - whether to put a newline character before dir (when argc > 1)
////    detailed - zero for only type and name, non-zero for details (ls -l);
////               the value is passed to list_file afterwards
////    show_hidden - flag describing whether to show files starting with a
////                  dot (.), or not
////    recursive - iterate recursively over children directories or not
////----------------------------------------------
void list_directory(const char *dir_path, int multiple, int detailed, int show_hidden, int recursive) {
    DIR *dir = opendir(dir_path);

    if (!dir) {
        perror(dir_path);
        exit(EXIT_FAILURE);
    }

    // Since stat is a system call, has disk I/O and would be called 3*n times (1. calculate total blocks,
    // 2. call list_file for each dir entry, 3. opendir again and call recursively), caching
    // the stats and paths in RAM for faster execution.
    file_stat_vector_t file_stat_vector;
    vector_init(&file_stat_vector);

    blkcnt_t dir_total_blocks = 0;

    indent_info_t indent_info;
    indent_info.largest_hardlinks_count = 0;
    indent_info.largest_file_size = 0;
    indent_info.longest_username = 0;
    indent_info.longest_groupname = 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!show_hidden && entry->d_name[0] == '.') {
            continue;
        }

        char* full_path = (char *)malloc(strlen(dir_path) + strlen(entry->d_name) + 2);
        if (full_path == NULL) {
            fprintf(stderr, "Could not allocate memory for absolute path concatenation");
            exit(EXIT_FAILURE);
        }

        if (sprintf(full_path, "%s/%s", dir_path, entry->d_name) < 0) {
            fprintf(stderr, "Could not concatenate absolute path");
            exit(EXIT_FAILURE);
        }

        struct stat entry_stat;
        if (stat(full_path, &entry_stat)) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        vector_push(&file_stat_vector, entry_stat, full_path);

        dir_total_blocks += entry_stat.st_blocks;

        indent_info.largest_file_size = max(entry_stat.st_size, indent_info.largest_file_size);
        indent_info.largest_hardlinks_count = max(entry_stat.st_nlink, indent_info.largest_hardlinks_count);
        indent_info.longest_username = max(strlen(get_username(entry_stat)), indent_info.longest_username);
        indent_info.longest_groupname = max(strlen(get_groupname(entry_stat)), indent_info.longest_groupname);
    }

    if (closedir(dir)) {
        perror("closedir");
        exit(EXIT_FAILURE);
    }

    if (multiple) {
        printf("%s:\n", dir_path);
    }

    if (recursive) {
        printf("\n%s:\n", dir_path);
    }

    if (detailed) {
        printf("total %lu\n", dir_total_blocks);
    }

    for (int i = 0; i < file_stat_vector.size; i++) {
        file_stat_vector_entry_t entry = vector_get(&file_stat_vector, i);
        list_file(entry.path, entry.stat, detailed, indent_info);
    }

    if (recursive) {
        char new_dir_path[PATH_MAX];
        for (int i = 0; i < file_stat_vector.size; i++) {
            file_stat_vector_entry_t entry = vector_get(&file_stat_vector, i);
            char *dup_path;
            if ((dup_path = strdup(entry.path)) == NULL) {
                perror("strdup");
                exit(EXIT_FAILURE);
            }

            char *file_name = basename(dup_path);

            if ((strcmp(file_name, ".") == 0) || (strcmp(file_name, "..") == 0)) {
                    continue;
            }

            if (S_ISDIR(entry.stat.st_mode)) {
                if (sprintf(new_dir_path, "%s/%s", dir_path, file_name) < 0) {
                    perror("sprintf");
                    exit(EXIT_FAILURE);
                }

                list_directory(new_dir_path, multiple, detailed, show_hidden, recursive);
            }
        }
    }

    vector_free(&file_stat_vector);
}

//--------------------------------------------
//// FUNCTION: get_filetype_letter
////    A utility function converting POSIX filetypes to a single character
//// PARAMETERS:
////    mode - mode_t struct for a filetype and mode (stat.st_mode)
////----------------------------------------------
char get_filetype_letter(int mode) {
    char letter;

    if (S_ISREG(mode)) {
        letter = '-';
    } else if (S_ISDIR(mode)) {
        letter = 'd';
    } else if (S_ISBLK(mode)) {
        letter = 'b';
    } else if (S_ISCHR(mode)) {
        letter = 'c';
    } else if (S_ISFIFO(mode)) {
        letter = 'p';
    } else if (S_ISLNK(mode)) {
        letter = 'l';
    } else if (S_ISSOCK(mode)) {
        letter = 's';
    } else {
        letter = '?';
    }

    return letter;
}

//--------------------------------------------
//// FUNCTION: count_digits
////    A utility function counting the number of digits in a number; used to
////    to calculate space indent to pad left hardlinks count and size.
//// PARAMETERS:
////    value - the number to count the digits of
////----------------------------------------------
unsigned count_digits(unsigned long long value) {
    if (value == 0) {
        return 1;
    }

    int count = 0;

    while (value != 0) {
       value /= 10;
       count++;
    }

    return count;
}

//--------------------------------------------
//// FUNCTION: get_repeated_spaces
////    A utility function used to repeat the space character for indentation.
//// PARAMETERS:
////    n - how many times to repeat
////----------------------------------------------
char* get_repeated_spaces(unsigned n) {
    char *str = (char *)malloc(n + 1);
    if (str == NULL) {
        fprintf(stderr, "Could not allocate memory for space indentation");
        exit(EXIT_FAILURE);
    }

    memset(str, ' ', n);
    str[n] = '\0';

    return str;
}

//--------------------------------------------
//// FUNCTION: max
////    Takes the biggest of two numbers.
//// PARAMETERS:
////    a - first number
////    b - second number
////----------------------------------------------
unsigned long long max(unsigned long long a, unsigned long long b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

//--------------------------------------------
//// FUNCTION: get_username
////    Extracts out the username of a file stat.
//// PARAMETERS:
////    file_stat - the stat of the file
////----------------------------------------------
char* get_username(struct stat file_stat) {
    struct passwd *passwd;
    if ((passwd = getpwuid(file_stat.st_uid)) == NULL) {
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }

    return passwd->pw_name;
}

//--------------------------------------------
//// FUNCTION: get_groupname
////    Extracts out the groupname of a file stat.
//// PARAMETERS:
////    file_stat - the stat of the file
////----------------------------------------------
char* get_groupname(struct stat file_stat) {
    struct group *group;
    if ((group = getgrgid(file_stat.st_gid)) == NULL) {
        perror("getgrgid");
        exit(EXIT_FAILURE);
    }

    return group->gr_name;
}

int main(int argc, char *argv[]) {
    int option = 0;

    int show_hidden_files = 0;
    int show_detailed_info = 0;
    int recursive = 0;

    while ((option = getopt(argc, argv, "alR")) != -1) {
        switch (option) {
            case 'a':
                show_hidden_files = 1;
                break;
            case 'l' :
                show_detailed_info = 1;
                break;
            case 'R':
                recursive = 1;
                break;
            default:
                fprintf(stderr, "Usage: ./ls [alR] [dir_path]");
                return -1;
        }
    }

    if (argc == optind) {
        list_directory(".", 0, show_detailed_info, show_hidden_files, recursive);
    } else {
        int multiple_args = ((argc - optind) > 1) ? 1 : 0;

        struct stat path_stat;
        if (stat(argv[optind], &path_stat)) {
            perror(argv[0]);
            return -1;
        }

        for (int i = optind; i < argc; i++) {
            if (S_ISREG(path_stat.st_mode)) {
                indent_info_t indent_info;
                indent_info.longest_username = strlen(get_username(path_stat));
                indent_info.longest_groupname = strlen(get_groupname(path_stat));
                indent_info.largest_file_size = path_stat.st_size;
                indent_info.largest_hardlinks_count = path_stat.st_nlink;

                list_file(argv[i], path_stat, show_detailed_info, indent_info);
            } else if (S_ISDIR(path_stat.st_mode)) {
                list_directory(argv[i], multiple_args, show_detailed_info, show_hidden_files, recursive);
            }

            if (i + 1 < argc) {
                if (stat(argv[i + 1], &path_stat)) {
                    perror(argv[0]);
                    return -1;
                }

                if (S_ISDIR(path_stat.st_mode)) {
                    printf("\n");
                }
            }
        }
    }

    return 0;
}
