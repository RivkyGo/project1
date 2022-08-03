#include "Input.h"
char command[SIZE_OF_LINE];

int jumpSpace(char * line, int index)/*Input: pointer to arry, and starting poin. Output: point after starting text. Discounts: (1)*/
{
	char c;
	while((c = line[index]) != '\n' && isspace(c))
		index++;
	return index;
}

int getCommand(char * line, int index)
{
	int i =0;
	while(!isspace(line[index]))
	{
		command[i] = line[index];
		index ++;
		i ++;	
	}
	command[i] = '\0';
	return index;

}
