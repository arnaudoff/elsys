#include <stdio.h>

void render_tree(int rows)
{
	int i;
	int space = rows - 1;
	int stars = 1;
	while (space >= 0)
	{
		for (i = 0; i < space; i++)
		{
			printf(" ");
		}
		for (i = 0; i < stars; i++) 
		{
			printf("*");
		}
		space = space - 1;
		printf("\n");
		stars = stars + 2;
	}
}

int main() 
{
	render_tree(4);
	render_tree(7);
	render_tree(11);
	return 0;
}