#include "macroList.h"

ptrMacro headMacroList = NULL;

void addToMacroList(char * name)
{
    ptrMacro new;
	new=(ptrMacro) malloc(sizeof(macroLine)); /*Allocation of memory*/
	if(!new)
	{ 
		printf("\n cannot allocate memory to a new macro \n");
	 	exit(0);
	}
	strcpy(new-> macroName, name);
	new->next = headMacroList;
    headMacroList = new;
    headMacroList -> index = 0;
	return;
}

ptrMacro sameMacro(char * macro_name)
{
    ptrMacro p = headMacroList;
	while(p) 					/*As long as p1 is not equal to null*/
	{
		if(!strcmp(macro_name ,p -> macroName))
			return p;
		p = p-> next;
	}
	return NULL;
}

void freeMacroList()			/*Memory release*/
{
	ptrMacro p = headMacroList;

	while(p !=NULL)
	{
        p = headMacroList;
        if(p) {
            headMacroList = p->next;
            free(p);
        }
	}
}
void writesFile(FILE * file, char * line)
{
    int i=0;
    while( line[i]!='\0')
    {

        fputc(line[i], file);
        i++;
    }
}

