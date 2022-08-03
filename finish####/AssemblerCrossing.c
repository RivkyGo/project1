#include "AssemblerCrossing.h"
#include "convertText.h"
extern int flagSymbol;
extern char command[SIZE_OF_LINE];
extern ptSymbol headSymbol;
extern ptDCList headDcList;
extern ptICList headIcList;
extern ptICList ptr;
int IC = 0, DC = 0;
char line[SIZE_OF_LINE];
char text[]= "test2.am";/**/
int entryFlag = 0;
int externFlag = 0;

/*int main()
{
	cross1(text);
	return 0;
	
}*/

int cross1(char * name_of_file, char * shortFileName)
{
	int row = 0, index = 0, startIndex = 0, opcode , error =0 , result =0,directiveType;
	FILE * file;
	file = fopen(name_of_file, "r");
	if (!file)
	{
		printf("error - %s no such file or directory\n",name_of_file);
		return -1;
	}
	while(!feof(file)) {
        if(IC +DC +MEMORY_ACCESS >= SIZE_OF_RAM)
        {
            printf("Memory exception");
            break;
        }
        row++;
        flagSymbol = 0;
        index = 0;
        startIndex = 0;
        fgets(line, SIZE_OF_LINE, file);
        if (!feof(file)) {
            index = jumpSpace(line, index);
            if (line[index] != '\n' && line[index] != ';') {
                startIndex = index;
                index = getCommand(line, index);/*At this point we have a word within the command*/
                result = islabel(command, row, startIndex);
                if (flagSymbol) {
                    index = jumpSpace(line, index);
                    index = getCommand(line, index);
                }
                if (result == -2)
                    error = 1;
                directiveType = isDirective(command, row, startIndex);
                opcode = isOrder(command);
                if (directiveType >= 0) {
                    if (flagSymbol) {
                        headSymbol->type = 1;
                        headSymbol->location = DC;
                    }
                    if (directiveType == 0) /* data*/
                        result = directiveData(line, index, row);
                    else if (directiveType == 1) /* string*/
                        result = directiveString(line, index, row);
                    else if (directiveType == 2) /* struct*/
                        result = directiveStruct(line, index, row);
                    else if (directiveType == 3) /* entry*/
                    {
                        if(flagSymbol) {
                            ptDCList pt;
                            pt = headDcList;
                            headDcList = headDcList->next;
                            free(pt);
                        }
                        entryFlag = 1;
                    }
                    else if(directiveType == 4 )/* extern*/
                    {
                        if(flagSymbol) {
                            ptDCList pt;
                            pt = headDcList;
                            headDcList = headDcList->next;
                            free(pt);
                        }
                        result = directiveExtern(line, index, row);
                        externFlag = 1;
                    }
                    if (result == -2)
                        error = 1;
                } else if (opcode >= 0) {
                    if (flagSymbol) {
                        headSymbol->type = 0;
                        headSymbol->location = IC + MEMORY_ACCESS;
                    }
                    if (opcode == 14 || opcode == 15)
                        result = instrucGroup3(opcode, line, index, row);
                    else if ((opcode >= 0 && opcode <= 3) || opcode == 6)
                        result = instrucGroup1(opcode, line, index, row);
                    else
                        result = instrucGroup2(opcode, line, index, row);
                    if (result == -2)
                        error = 1;
                } else {
                    printf("error in row %d index  %d: Command %s - not recognized \n", row, startIndex, command);
                    error = 1;
                }
            }
        }
    }
	if(error==1)
	{
		freeListDC();
		freeListIC();
		freeSymbolList();
        return -2;
	}
    addSymbolIc();
    addIc();
    fclose(file);
    printIc();
    printSymbol();/**/
    result = cross2(name_of_file , shortFileName);
    if(result == -2)
        return -2;
	return 0;
}

int cross2(char * name_of_file , char * shortFileName)
{
    int row = 0, index = 0, startIndex = 0, opcode , error =0 , result =0,directiveType;
    char * label;
	FILE * file;
    ptr = headIcList;
    file = fopen(name_of_file, "r");
    if (!file)
    {
        printf("error - %s no such file or directory\n",name_of_file);
        return -1;
    }
    while(!feof(file))
    {
        row++;
        flagSymbol = 0;
        index = 0;
        startIndex = 0;
        fgets(line, SIZE_OF_LINE, file);
        if (!feof(file))
        {
            index = jumpSpace(line, index);
            if (line[index] != '\n' && line[index] != ';')
            {
                startIndex = index;
                index = getCommand(line, index);/*At this point we have a word within the command*/
                label = strrchr(command, ':');
                if(label)
                {
                    index = label - command + startIndex +1;
                    index = jumpSpace(line, index);
                    index = getCommand(line, index);
                }
                directiveType = isDirective(command, row, startIndex);
                if(directiveType >=0)
                {
                    if(directiveType == 3)
                    {
                        if(isEntry(line ,index, row))
                            error = 1;
                    }
                }
                else
                {
                    opcode = isOrder(command);
                    if (opcode == 14 || opcode == 15)
                       ptr = ptr -> next;
                    else if ((opcode >= 0 && opcode <= 3) || opcode == 6)
                        result = labelGroup1( line, index, row);
                    else
                        result = labelGroup2( line, index, row);
                }
                if(result == -2)
                    error = 1;
            }
        }
    }
    if(error==1)
    {
        freeListDC();
        freeListIC();
        freeSymbolList();
        fclose(file);
        return -2;
    }
    printIc();
    convertToObject(shortFileName);
    printSymbol();/**/
    if(externFlag == 1)
        convertToExtern(shortFileName);
    if(entryFlag == 1)
        convertToEntry(shortFileName);
    freeListDC();/**/
    freeListIC();/**/
    freeSymbolList();/**/
    freeExternalList();/**/
    fclose(file);
    return 0;
}
