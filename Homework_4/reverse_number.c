#include <stdio.h>

int main() 
{
	int rmider, i;
	int numbers[5] = {5170984, 406567, 368750, 2369510, 2183293};
	
	for (i = 0; i < 5; i++)
	{
		while (numbers[i] > 0) 
		{
			rmider = numbers[i] % 10;
			printf("%d", rmider);
			numbers[i] = numbers[i] / 10;
		}
		printf("\n");
	}

	return 0;
}