#include <stdio.h>

int main()
{
	unsigned long int f_prev = 2;
	unsigned long int s_prev = 1;
	int count;
	printf("%lu, %lu", f_prev, s_prev);

	for (count = 1; count <= 49; count++)
	{
		f_prev = f_prev + s_prev;
		s_prev = f_prev + s_prev;
		printf(", %lu, %lu", f_prev, s_prev);
	}
	printf("\n");
	return 0;
}