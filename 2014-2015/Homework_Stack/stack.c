#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "stack.h"

void stack_init(stack_t *stack, int size, stack_error_t *error)
{
	stack_element *contents;

	contents = (stack_element *)malloc(sizeof(stack_element) * size);

	if (contents == NULL)
	{
		*error = ESTACKNOMEM;
		return;
	}

	*error = SUCCESS;
	stack->contents = contents;
	stack->size = size;
	stack->top = -1;
}

int stack_is_empty(stack_t *stack)
{
	return stack->top < 0;
}

int stack_is_full(stack_t *stack)
{
	return stack->top >= stack->size - 1;
}

int stack_get_current_size(stack_t *stack)
{
	return stack->top + 1;
}

void stack_push(stack_t *stack, stack_element element, stack_error_t *error)
{
	if (stack_is_full(stack)) 
	{
		*error = ESTACKFULL;
	}
	
	*error = SUCCESS;
	stack->contents[++stack->top] = element;
}

stack_element stack_pop(stack_t *stack, stack_error_t *error)
{
	if (stack_is_empty(stack))
	{
		*error = ESTACKEMPTY;
		return;
	}

	*error = SUCCESS;
	return stack->contents[stack->top--];	
}

void stack_destroy(stack_t *stack)
{
	free(stack->contents);

	stack->contents = NULL;
	stack->size = 0;
	stack->top = -1;
}

const char* stack_strerror(stack_error_t error)
{
	switch (error)
	{
		case ESTACKNOMEM:
			return "Insufficient memory.";
		case ESTACKFULL:
			return "Stack overflow.";
		case ESTACKEMPTY:
			return "Stack is empty.";
	}
}