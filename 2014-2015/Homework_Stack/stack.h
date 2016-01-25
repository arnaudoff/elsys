#ifndef STACK_H_
#define STACK_H_

typedef char stack_element;

typedef struct stack 
{
	stack_element *contents;
	int top;
	int size;
} stack_t;

typedef enum stack_error 
{
	ESTACKNOMEM, 
	ESTACKFULL,
	ESTACKEMPTY,
	SUCCESS
} stack_error_t;

const char* stack_strerror(stack_error_t error);

void stack_init(stack_t *stack, int size, stack_error_t *error);

int stack_is_full(stack_t *stack);

int stack_is_empty(stack_t *stack);

int stack_get_current_size(stack_t *stack);	

void stack_push(stack_t *stack, stack_element element, stack_error_t *error);

stack_element stack_pop(stack_t *stack, stack_error_t *error);

void stack_destroy(stack_t *stack);

#endif