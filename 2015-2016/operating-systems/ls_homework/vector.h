#include <sys/stat.h>

#define VECTOR_INITIAL_CAPACITY 16

typedef struct {
  int size;
  int capacity;
  struct stat *stats;
  char **paths;
} file_stat_vector_t;

typedef struct {
    struct stat stat;
    char *path;
} file_stat_vector_entry_t;

void vector_init(file_stat_vector_t *vector);

void vector_push(file_stat_vector_t *vector, struct stat file_stat, char *file_path);

file_stat_vector_entry_t vector_get(file_stat_vector_t *vector, int index);

void vector_resize(file_stat_vector_t *vector);

void vector_free(file_stat_vector_t *vector);
