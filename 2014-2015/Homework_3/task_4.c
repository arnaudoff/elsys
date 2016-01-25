#include <stdio.h>

int main() 
{
	int n, i;
	n = 1;
	for (i = 1; i <= 10; i++) 
	{
		n = n * i;
	}
	printf("%d\n", n);
	return 0;
}