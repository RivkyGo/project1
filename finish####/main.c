#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "convertText.h"
#include "macroList.h"
#define SIZE_OF_LINE 81
extern ptrMacro headMacroList;

void macroRetir(FILE *, FILE *);
enum bool {false,true};
enum bool macro_flag = false;
enum bool error = false;

/*int main(int argc, char const *argv[])
{	
	FILE * file;
	FILE * newFile;
	int i;
	char *fullFileName = NULL,* fullFileNewName =NULL;;	
	
	if(argc <= 1)
	{
		printf("No File where typed.\n");
		exit(0);
	}
	for	(i=1; i<argc; i++)
	{	
		fullFileName =NULL;
		fullFileName = concat(argv[i],".as\0");		
		if(fullFileName)
		{
			printf(" -- File: %s -- \n",fullFileName);
			file = fopen(fullFileName, "r");
			fullFileNewName = concat(argv[i],".am\0");	
			newFile = fopen(fullFileNewName, "w");
			if (file && newFile)
			{
				macroRetir(file, newFile);
			}
		}
		fclose(file);
		fclose(newFile);
	}
	return 0;
}*/

int main()
{
    char text[]= "y";
    FILE * file;
    FILE * newFile;

    char * fullFileName = NULL,* fullFileNewName =NULL;

        fullFileName = concat(text,".as\0");

        if(!rename(text , fullFileName))
        {
            printf(" -- File: %s -- \n",text);
            file = fopen(fullFileName, "r");
            fullFileNewName = concat(text,".am\0");
            newFile = fopen(fullFileNewName, "w");
            if (file && newFile)
            {
                macroRetir(file, newFile);
            }
        }
        fclose(file);
        fclose(newFile);
        free(fullFileName);
        cross1(fullFileNewName , text);
        free(fullFileNewName);
    return 0;
}

void macroRetir (FILE *source, FILE *target)
{
	int i = 0, row = 0, index=0;
    char buffer[SIZE_OF_LINE];
	char firstWord[SIZE_OF_LINE];
    ptrMacro pt;
	while (!feof(source))
    {
        fgets(buffer, SIZE_OF_LINE, source);
        row++;
        index = 0;
        i=0;
        /*if (!feof(source))*/
        {
            index = jumpSpace(buffer, index);
            while (index < SIZE_OF_LINE && !isspace(buffer[index])  ) {
                firstWord[i] = buffer[index];
                i++;
                index++;
            }
            firstWord[i] = '\0';
            if (!strcmp(firstWord, "macro"))
            {
                if (macro_flag == false) {
                    macro_flag = true;
                    i = 0;
                    index = jumpSpace(buffer, index);
                    while (!isspace(buffer[index]) && i < SIZE_OF_LINE) {
                        firstWord[i] = buffer[index];
                        i++;
                        index++;
                    }
                    firstWord[i] = '\0';
                    if(isOrder(firstWord)>=0 || isRegister(firstWord) != -1|| isPureDirective(firstWord)>=0)
                        error = true;
                    if (!strcmp(firstWord, "")) {
                        printf("error in line %d - macro statement without name.\n", row);
                        error = true;
                    }
                    else if (sameMacro(firstWord))
                    {
                        printf("error in line %d index %d - Conflict in the macro type, \na macro with the same name was previously defined",
                               row, index);
                        error = true;
                    }
                    index = jumpSpace(buffer, index);
                    if (buffer[index] != '\n') {
                        printf("error in line %d index %d - excess characters after the macro name", row, index);
                        error = true;
                    }
                    if(!error)
                        addToMacroList(firstWord);

                }
                else
                {
                    printf("error in line %d index %d - set up a nested macro.", row, index);
                    exit(0);
                }
            }
            else if (!strcmp(firstWord, "endmacro"))
            {
                if (macro_flag == false) {
                    printf("error in line %d index %d - attempt to close undeclared macro.", row, index);
                }
                else {
                    index = jumpSpace(buffer, index);
                    if (buffer[index] != '\n') {
                        printf("error in line %d index %d - excess characters after the endmacro.\n", row, i);
                    }
                    else {
                        headMacroList->content[headMacroList->index] = '\0';
                        headMacroList->index = headMacroList->index + 1;
                        macro_flag = false;
                    }
                }
            }
            else if (macro_flag == true )
            {
                if(!error) {
                    i = 0;
                    while (i < SIZE_OF_LINE && buffer[i] != '\n' && headMacroList->index < SIZE_CONTENT) {
                        headMacroList->content[headMacroList->index] = buffer[i];
                        i++;
                        headMacroList->index = headMacroList->index + 1;
                    }
                    headMacroList->content[headMacroList->index] = '\n';
                    headMacroList->index = headMacroList->index + 1;
                }
            }
            else
            {
                pt = sameMacro(firstWord);
                if (pt)
                    writesFile(target, pt->content);
                else
                    fputs(buffer, target);
            }

        }
    }
    fputc('\n', target);
    freeMacroList();
}
















