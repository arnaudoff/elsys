#include <stdio.h>
#define MAX_STRING_SIZE 64

void print_rot13(char word[MAX_STRING_SIZE])
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
   	char buf[MAX_STRING_SIZE+2]; // c/d/q + space	
	while (buf[0] != 'q') 
	{
		printf("Enter option: ");
    	int i = 0;
    	while((buf[i++] = getchar()) != '\n' && i < MAX_STRING_SIZE+2);
    	buf[i] = '\0';
    	
    	char word[i];
    	for (i = 0; buf[i] != '\0'; i++)
    	{
    		word[i] = buf[i + 2];
    	}
    	word[i] = '\0';
    	switch (buf[0])
    	{
    		case 'c':
    			print_rot13(word);
    			printf("\n");
    			break;
    		case 'd':
    			/*
    				http://en.wikipedia.org/wiki/ROT13
    				"Because there are 26 letters in the English alphabet and 26 = 2 × 13, the ROT13 function is its own inverse." 
    			*/
    			print_rot13(word); 
    			printf("\n");
    			break;
    		default:
    			break;
    	}
    }
	return 0;
}