#include <stdio.h>


int main() 
{
	int i, j, k, next, prime;
	i = 1;
	k = 2;
	prime = 1;
	
	while (i <= 10)
	{
		for (j = 2; j < k; j++)
		{
			if (k % j == 0) prime = 0;
		}
		next = k + 2;
		for (j = 2; j < next; j++) {
			if (next % j == 0) prime = 0;		
		}
		if (prime == 0) prime = 1; 
		else 
		{ 
			printf("(%d, %d)\n", k, next);
			i = i + 1;
		}

		k = k + 1;
	}

	return 0;
}
