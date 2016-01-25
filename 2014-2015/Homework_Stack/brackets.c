#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

#define INPUT_STRING_SIZE 256
#define STACK_SIZE 1000

int is_balanced(stack_t *stack, const char *str)
{
	stack_error_t serror;	
	int i;
	for (i = 0; str[i] != '\0'; i++)
	{
		switch(str[i])
		{
			case '(':
			case '{':
			case '[':
				stack_push(stack, str[i], &serror);
				break;
			case ']':
				if (stack_is_empty(stack) || stack_pop(stack, &serror) != '[') return -1;
				break;
			case '}':
				if (stack_is_empty(stack) || stack_pop(stack, &serror) != '{') return -1;
				break;			
			case ')':
				if (stack_is_empty(stack) || stack_pop(stack, &serror) != '(') return -1;
				break;
		}

		if (serror != SUCCESS)
		{
			fprintf(stderr, "Could not push/pop to/from stack. Error: %s\n", stack_strerror(serror));
			exit(1);
		}		
	}

	if (stack_is_empty(stack))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int main()
{
	char str[INPUT_STRING_SIZE];
	scanf("%s", str);

	stack_t stack;
	stack_error_t serror;
	stack_init(&stack, STACK_SIZE, &serror);
	
	if (serror != SUCCESS)
	{
		fprintf(stderr, "Could not initialize stack. Error: %s\n", stack_strerror(serror));
		exit(1);
	}

	// printf("The brackets are %sbalanced.\n", (is_balanced(str) == 0) ? "" : "NOT ");
	printf("%d\n", is_balanced(&stack, str));
	stack_destroy(&stack);

	return 0;
}