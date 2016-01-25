#include <stdio.h>
#define MAX_STRING_SIZE 64

/* rot13.c */
void printdecrypted(char word[MAX_STRING_SIZE])
{
	int i;
	for (i = 0; word[i] != '\0'; i++)
	{
		if(isupper(word[i])) 
		{
			if (word[i] >= 'A'  && word[i] <= 'M') 
			{
				word[i] += 13;
				printf("%c", word[i]);
			} 
			else if(word[i] >= 'N' && word[i] <= 'Z') 
			{
				word[i] -= 13;
				printf("%c", word[i]);
			} 
			else if(word[i] == ' ') 
			{
				printf(" ");
			}
		}
		else 
		{
			if (word[i] >= 'a'  && word[i] <= 'm') 
			{
				word[i] += 13;
				printf("%c", word[i]);
			} 
			else if(word[i] >= 'n' && word[i] <= 'z') 
			{
				word[i] -= 13;
				printf("%c", word[i]);
			} 
			else if(word[i] == ' ')
			{
				printf(" ");
			}	
		}
	}
}

int main()
{
	char text[MAX_STRING_SIZE];
	scanf("%64s", text);
	int len = 0;
	int i;
	while (text[len] != '\0') len++;
	for (i = 0; i < len/2; i++)
	{
		char tmp = text[i];
		text[i] = text[len-i-1];
		text[len-i-1] = tmp;
	}
	printdecrypted(text);
	printf("\n");
	return 0;
}