#include <stdio.h>

int main()
{
	int range;
	int len;
	printf("Enter range: ");
	scanf("%d", &range);
	int array[range];
	printf("Enter length: ");
	scanf("%d", &len);
	printf("Enter array: ");
	int i = 0;
	while (i < len && scanf("%d", &array[i]) == 1)
	{
  		i++; // http://stackoverflow.com/questions/9599794/putting-numbers-separated-by-a-space-into-an-array
  	}
	printf("\n");
	printf("Histogram: ");
	printf("\n");
	int count;
	int j, k;
	for (i = 0; i <= range; ++i)
	{
		printf("%d: ", i);
		count = 0;
		for (j = 0; j < len; ++j)
		{
			if (array[j] == i) count++;
		}
		for (k = 0; k < count; ++k)
		{
			printf("*");
		}
		printf("\n");
	}
	return 0;
}