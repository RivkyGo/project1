#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE_MACRO_NAME 81
#define SIZE_CONTENT 810



typedef struct  MacroList * ptrMacro;

typedef struct MacroList{
    char content[SIZE_CONTENT];
    int index;
    char macroName[SIZE_MACRO_NAME];
    ptrMacro next;
}macroLine;


/*typedef struct  lineMacro * ptrLine;

typedef struct lineMacro{
		char content[SIZE_MACRO_NAME];
        ptrLine next;
		}macroLine;

typedef struct macroList * ptrMacro;

typedef struct macroList{
    char macroName[SIZE_MACRO_NAME];
    ptrLine macroLine;
    ptrMacro next;
}macro;*/

void writesFile(FILE *, char *);		/*Opens the file for updating and inserts a line into it*/
void addToMacroList(char * ); /* pointer to the head of list, name of macro, pointer to text file with the content of the macro*/
void freeMacroList();
ptrMacro sameMacro(char *);			/* input: one string.  Output: '1'if there was a macro with the same name as the string in the list of macros and another '0'*/
