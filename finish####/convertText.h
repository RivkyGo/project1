#ifndef CONVERTTEXT_H
#define CONVERTTEXT_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "checkText.h"
#include "AssemblerCrossing.h"
#include "Input.h"
#define SIZE_BASE 32


char* concat(const char *s1, const char *s2);       /* input: two strings.  Output: one string that is the combination of both strings*/
void ConvertToBase(int);
int convertToObject(char * );
void convertToEntry(char * );
void convertToExtern(char * );
#endif