#ifndef CHECKTEXT_H
#define CHECKTEXT_H
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "Input.h"

#define MEMORY_ACCESS 100
#define SIZE_LABEL_NAME 31
#define SIZE_STRUCT 33

typedef struct symbolNode * ptSymbol;
typedef struct symbolNode{
		char label_name[SIZE_LABEL_NAME];
		int location;
		int type;
		int typeE;
		ptSymbol next;
		}symbol;

typedef struct externalNode * ptExtern;
typedef struct externalNode{
    char label_name[SIZE_LABEL_NAME];
    int location;
    ptExtern next;
}externList;

typedef struct word{
			unsigned int are: 2;
			unsigned int rt: 4;
			unsigned int rs: 4;
			unsigned int opcode: 4;
			unsigned int number: 8;
			unsigned int string: 10;
			unsigned int addressS: 2;
			unsigned int addressT: 2;
			unsigned int wordType: 2;
		}coding;

typedef struct ICList * ptICList;

typedef struct ICList{
			coding ramWord;
			int decimalAddress;
			ptICList next;
			}listIC;	

typedef struct DCList * ptDCList;

typedef struct DCList{
			coding ramWord;
			int decimalAddress;
			ptDCList next;
			}listDC;	
		
int isDirective (char * ,int ,int ); /* Input: an array containing the first word in the line, line number, index of the first word
					Output: if this is a prompt statement - returns the index of the prompt
						or the directive statement is false returns -2
						or it is not a directive statement returns -1*/
int isPureDirective(char *);
int isOrder (char *);  /*Input: an array containing the first word in the line, line number, index of the first word
			Output: if it is an instruction sentence - returns the index of the instruction
				or it is not an instruction statement returns -1*/
int islabel(char *, int , int );
void addToSymbolList(char *);
void addToExternalList(ptSymbol,int );
int sameSymbol(char * );
void freeSymbolList();
void freeExternalList();

int isRegister(char *);/*Input: a string, and checks if the word the string contains is the name of a register
			Output: returns the register number if it exists.
			otherwise returns -1*/
int correctDigit( char * );/*Input: String
				Output: 0 if the string is not an integer
				otherwise 1.*/
void addToListIC();
void freeListIC();
int address(char *);
int instrucGroup1(int , char *, int ,int);
void memoryCoding(int , int , char *, char *);
int instrucGroup2(int , char *, int ,int);
int instrucGroup3(int , char *, int ,int);

int directiveData(char *, int , int);
int directiveString(char *, int , int );
void addToListDC();
void freeListDC();
void addIc();
int directiveStruct(char *, int , int );
int directiveExtern(char * ,int ,int );
void addSymbolIc();
int isEntry(char *  , int , int);
int memoryCoding2(int , int ,int , char * );
int labelGroup1( char *, int, int );
int labelGroup2( char *, int, int );
void printSymbol(); /**/
void printIc();  /**/




#endif