#include <stdio.h>
#include <stdlib.h>

struct vector_t 
{
	int *data;
	int index, size;
};

void vector_init(struct vector_t*);

void vector_destroy(struct vector_t*);

int vector_get_size(struct vector_t);

int vector_resize(struct vector_t*);

int vector_at(struct vector_t, int index);

void vector_push_back(struct vector_t*, int value);

int vector_pop_back(struct vector_t*);

int main() 
{
	struct vector_t vector;
	vector_init(&vector);
	printf("Initial capacity: %d\n", vector.size);	

	int i;
	for (i = 0; i < 5; i++)
	{
		vector_push_back(&vector, i);
		printf("Current capacity: %d Value: %d\n", vector.size, vector_at(vector, i));
	}
	vector_destroy(&vector);
	return 0;
}

void vector_init(struct vector_t* v) 
{
	v->size = 5;
	v->index = 0;
	v->data = (int*) malloc(v->size * sizeof(int));
}

void vector_destroy(struct vector_t* v)
{
	free(v->data);
	v->size = 0;
	v->index = 0;
}

int vector_get_size(struct vector_t v)
{
	return v.index;
}

void vector_push_back(struct vector_t* v, int value)
{
	int res = vector_resize(v);
	v->data[v->index++] = value;
}

int vector_at(struct vector_t v, int index)
{
	if (index < 0 || index >= vector_get_size(v))
	{
		// This cannot be handled correctly if the vector contains -1..
		return -1;
	}

	return v.data[index];
}

int vector_pop_back(struct vector_t* v)
{
	if (vector_get_size(*v) == 0)
	{
		return -1;
	}

	int res = vector_resize(v);
	return v->data[--v->index];
}

// Returns -1 if the resizing has failed and 0 if succeeded (aka boolean)
int vector_resize(struct vector_t* v)
{
	if (v->index == v->size)
	{
		v->size = v->size * 2;
		return 0;
	}
	else if (v->index < v->size / 2)
	{
		v->size = v->size / 2;		
		return 0;
	}

	return -1;
}