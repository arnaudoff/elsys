#include <stdio.h>


int checkForMagic(int square[999][999], int n)
{
	int i, j;
	int magic = 1;

	int sumd[2]; sumd[0] = 0; sumd[1] = 0;
	for (i = 0; i < n; i++)
	{	
		for (j = 0; j < n; j++)
		{
			if (i == j) sumd[0] = sumd[0] + square[i][j];
		}
		sumd[1] += square[i][n-1-i];
	}
	if (sumd[0] != sumd[1]) magic = 0;

	int sumr;
	for (i = 0; i < n; i++) 
	{
		sumr = 0;
		for (j = 0; j < n; j++) sumr += square[i][j];
		if (sumr != sumd[0]) magic = 0;	
	}

	int sumc;
	for (j = 0; j < n; j++)
	{
		sumc = 0;
		for (i = 0; i < n; i++) sumc += square[i][j];
		if (sumc != sumd[0]) magic = 0;
	}
	return magic;
}
int main()
{
	int n;
	scanf("%d", &n);
	if (n > 999) return 0;
	int square[999][999];
	int i, j;

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			scanf("%d", &square[i][j]);
		}
	}
	if (checkForMagic(square, n) != 0) printf("MAGIC\n");
	else printf("NO MAGIC\n");
	return 0;
}