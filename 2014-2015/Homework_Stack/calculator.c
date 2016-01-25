#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define INPUT_BUFFER_SIZE 256
#define MAX_TOKEN_COUNT 100
#define STACK_SIZE 1000

int evaluate_rpn(stack_t *numbers, char *expression[], int expr_len, int *is_valid_expression)
{
	stack_error_t serror;	
	int i;
	for (i = 0; i < expr_len; i++)
	{
		if (strncmp(expression[i], "+", 1) == 0 || strncmp(expression[i], "-", 1) == 0 || strncmp(expression[i], "*", 1) == 0 || strncmp(expression[i], "/", 1) == 0)
		{
			int first_number = stack_pop(numbers, &serror);
			int second_number = stack_pop(numbers, &serror);

			if (strncmp(expression[i], "+", 1) == 0) stack_push(numbers, first_number + second_number, &serror);
			if (strncmp(expression[i], "-", 1) == 0) stack_push(numbers, second_number - first_number, &serror);
			if (strncmp(expression[i], "*", 1) == 0) stack_push(numbers, first_number * second_number, &serror);			
		    if (strncmp(expression[i], "/", 1) == 0) stack_push(numbers, second_number / first_number, &serror);
		}
		else
		{
			int number;
			sscanf(expression[i], "%d", &number);
			stack_push(numbers, number, &serror);
		}

		if (serror != SUCCESS)
		{
			fprintf(stderr, "Could not push/pop to/from stack. Error: %s\n", stack_strerror(serror));
			exit(1);
		}		
	}

	if (stack_get_current_size(numbers) != 1)
	{
		*is_valid_expression = 0;
		return;
	}

	*is_valid_expression = 1;
	return stack_pop(numbers, &serror);
}

int main()
{
	char buffer[INPUT_BUFFER_SIZE];
	scanf("%[^\n]s", buffer);

	char *expression[MAX_TOKEN_COUNT];
	const char *delimiter = " ";

	char *token;
	token = strtok(buffer, delimiter);  
	int expr_len = 0;
	while (token != NULL) 
	{
		expression[expr_len] = token;
		token = strtok(NULL, delimiter);
		expr_len++;
	}

	stack_t numbers;
	stack_error_t serror;

	stack_init(&numbers, STACK_SIZE, &serror);

	if (serror != SUCCESS)
	{
		fprintf(stderr, "Could not initialize stack. Error: %s\n", stack_strerror(serror));
		exit(1);
	}

	int is_valid_rpn;
	int res = evaluate_rpn(&numbers, expression, expr_len, &is_valid_rpn);
	if (is_valid_rpn)
	{
		printf("%d\n", res);
	}
	else
	{
		fprintf(stderr, "Invalid reverse polish notation expression given!\n");
	}

	stack_destroy(&numbers);

	return 0;
}