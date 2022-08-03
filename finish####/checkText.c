#include "checkText.h"

extern int IC;
extern int DC;
ptSymbol headSymbol = NULL;
ptICList headIcList = NULL;
ptExtern headExternList = NULL;
ptICList tailIcList = NULL;
ptDCList headDcList = NULL;
ptDCList tailDcList = NULL;
ptICList ptr = NULL;

enum status {OFF,ON};
enum ARE {A, E, R , entry};

enum status flagSymbol = OFF;

char *directives[5] = {"data\0","string\0","struct\0","entry\0","extern\0"};
char *orders[16] = {"mov\0","cmp\0","add\0","sub\0","not\0","clr\0","lea\0","inc\0","dec\0","jmp\0","bne\0","get\0","prn\0","jsr\0","rts\0","hlt\0"};
char *registers[8] = {"r0" , "r1" , "r2" , "r3" , "r4" , "r5" , "r6" , "r7"};

int isDirective (char *command, int row, int index)
{
	int i;
	char *pt;
	if(command[0] == '.')
	{
		pt=command;
		pt = pt + 1;  /* The array of commands will have a function without the dot*/
		for (i=0;i<5;i++)
		{
			if(strcmp(pt, directives[i])==0)
		        	return i;  /*return the index of directive sentence*/
		}
		printf("error in row %d index  %d - %s is not a directive sentence\n",row, index+1 ,pt);
		return -2;   /*When there is an error in the prompt it will return -2*/
	}
	else 
	{
		for (i=0;i<5;i++)
		{
			if(strcmp(command, directives[i])==0)
			{
				printf("Error in row %d index  %d: missing a point before the directive statement\n",row, index);
		        	return -2;  
			}
		}
	}
	return -1; /*when it is not a prompt statement will return -1*/          
}

int isPureDirective(char * command)
{
    int j=0;
    while(j<5)
    {
        if(!strcmp(command ,directives[j]))
            return j;
        j++;
    }
    return -1;
}

int isOrder (char *command)
{
	int i;
	for (i=0;i<16;i++)
		{
			if(strcmp(command, orders[i])==0)
		        	return i;  /*return the opcode */
		}
	return -1;
}

int islabel(char *command, int row, int index)
{
	int i=0,j=0, last;
	char  *ptr = command, *flagColon;
	flagColon = strrchr(command, ':');
	if(flagColon)
	{
        last = flagColon - command;
		if(command[last+1]!= '\0')
		{
			printf("error in row %d index  %d: %s - does not match any sentence syntax\n",row, index, command);
			return -2;
		}
        command[last]='\0';
		if(!isalpha(command[0]))
		{
			printf("error in row %d index  %d: %s - in the  first character in the label no letter was found\n",row, index, command);
			return -2;
		}		
		while(i < last)
		{
			if(!isalpha(command[i]) && !isdigit(command[i]))
			{
				printf("error in row %d index  %d: %s - non-number and non-letter character was found\n",row, index, command);
				return -2;
			}
			i++;
		}
		if(isOrder(command) != -1)
		{
			printf("error in row %d index  %d: %s - An attempt to define a label in an assembly language reserved word\n",row, index, command);
			return -2;
		}
		if(strlen(command)>30)
		{
			printf("error in row %d index  %d: %s - The length of the label is greater than 30\n",row, index, command);
			return -2;
		}
		if(sameSymbol(command))
		{
			printf("error in row %d index  %d: %s - Restatement of label in use detected\n",row, index, command);
			return -2;
		}
		if(isRegister(command) != -1)
		{
			printf("error in row %d index  %d: %s - using language reserved name in label definition\n",row, index, command);
			return -2;
		}
		while(j<5)
		{
			if(!strcmp(command ,directives[j]))
			{
				printf("error in row %d index  %d: %s - using language reserved name in label definition\n",row, index, command);
				return -2;
			}
			j++;
		}	 	 
		addToSymbolList(command);
		flagSymbol = 1;
		return index;
		
	}
	return -1; /* it is not a label*/
	
}

void addToSymbolList(char * name )
{
	ptSymbol new;
	new=(ptSymbol) malloc(sizeof(symbol)); /*Allocation of memory*/
	if(!new)
	{ 
		printf("\n cannot allocate memory to a new symbol \n");
	 	exit(0);
	}

	strcpy(new-> label_name , name);
	
	new->next = headSymbol;
	headSymbol = new;
	new -> typeE = R;
	return;
}

void addToExternalList(ptSymbol p, int location)
{
    ptExtern new;
    new=(ptExtern) malloc(sizeof(externList)); /*Allocation of memory*/
    if(!new)
    {
        printf("\n cannot allocate memory to a new symbol \n");
        exit(0);
    }

    strcpy(new-> label_name , p ->label_name );
    new ->location = location;
    new->next = headExternList;
    headExternList= new;
    return;
}

int sameSymbol(char * label_name)
{
	ptSymbol p = headSymbol;
	while(p != NULL) 					/*As long as p1 is not equal to null*/
	{
		if(!strcmp(label_name ,p -> label_name))
			return -2;
		p = p-> next;
	}
	return 0;
	
}


void freeSymbolList()			/*Memory release*/
{
	ptSymbol p;
	while(headSymbol)
	{
		p=headSymbol;
		headSymbol=p->next;
		free(p);
	}
}

void freeExternalList()			/*Memory release*/
{
    ptExtern p;
    while(headExternList)
    {
        p = headExternList;
        headExternList = p -> next;
        free(p);
    }
}
int isRegister(char * command)
{
	int i;
	for(i=0; i<8;i++)
	{
		if(strcmp(command, registers[i])==0)
			return i;
	}
	return -1;
}

int correctDigit( char * command)
{
	int i=0;
	if(command[i] == '-' || command[i] == '+')
		i++;
	while(command[i] != '\0')
	{
		if(!isdigit(command[i]))
			return 0;
        i++;
	}
	if(!strcmp(command,""))
		return 0;
	return 1;
}

void addToListIC()
{
	ptICList new;
	new=(ptICList) malloc(sizeof(listIC)); /*Allocation of memory*/
	if(!new)
	{ 
		printf("\n cannot allocate memory to a new symbol \n");
	 	exit(0);
	}
	new -> next =NULL;
	if(headIcList == NULL)
	{
		headIcList = new;
		tailIcList = headIcList;	
	}
	else
	{
	tailIcList->next = new;
	tailIcList = new;
	}
	tailIcList-> decimalAddress = IC + MEMORY_ACCESS;
	IC++;
	return;
}

void freeListIC()			/*Memory release*/
{
	ptICList p;
	while(headIcList)
	{
		p=headIcList;
		headIcList=p->next;
		free(p);
	}
}


int address(char * word)
{
	int i=0;
	char * ptr;
	if(word[0]  == '#')
	{
		if(!correctDigit(word+1))
			return -2;
		else
			return 0;
	}
	if(word[0] == 'r')
	{
		if(isRegister(word) != -1)
			return 3;
	}
	ptr =strchr(word, '.');
	if(ptr)
	{
		i = 0;
		if((*(ptr+1)=='1'||*(ptr+1)=='2') && *(ptr+2) =='\0'&& isalpha(word[0]))
			while(word[i]!='.')
			{
				if(!isalpha(word[i]) && !isdigit(word[i]))
					break;
				i++;
			}
		if(word[i] =='.')
			return 2;
		return -2;
	}
	if(isalpha(word[0]))
	{
		i = 0;
		while(word[i]!='\0')
		{
			if(!isalpha(word[i]) && !isdigit(word[i]))
				break;
			i++;
		}
		return 1;	
	}
	return -2;	
}


int instrucGroup1(int opcode, char *line, int index,int row)
{
	
	char rs[SIZE_STRUCT] , rt[SIZE_STRUCT];
	int i = 0, result = 0, addressS, addressT;
	index = jumpSpace(line,index);
	while (line[index] != ','  && line[index] != '\n' && line[index] != ' ')
	{
		rs[i] = line[index];
		i++;
		index++;
	}
	rs[i] = '\0';
	index = jumpSpace(line,index);
	if(line[index] == '\n')
	{
		printf("in the row %d in the index %d in commands of this type, two operands must be written\n", row, index);
		result = -2;
	}
	else if(line[index] != ',')
	{
		printf("a comma is missing in the row %d in the index %d \n", row, index);
		result = -2;
	}
	else
	{
		i = 0;
        index++;
        index = jumpSpace(line,index);
		while(!isspace(line[index]) && i< SIZE_STRUCT)
		{
			rt[i] = line[index];
			i++;
			index++;
		}
		rt[i] = '\0';
		index = jumpSpace(line,index);
		if(line[index] != '\n')
		{
			printf("in the row %d in the index %d There are more characters after the operands\n ", row, index);
			result = -2;
		}	
	}
	if(result == -2)
		return -2;  /* error in the sentence*/
	addressS = address(rs); /* which address is it*/
	addressT = address(rt); /* which address is it*/
	if(addressS == -2 || addressT == -2)
	{
		printf("Error in line %d One of the operands is invalid\n", row);
		return -2;
	}
	if((opcode == 0 || opcode == 2 || opcode == 3 || opcode == 6) && addressT == 0 )		
	{
		printf("Error in line %d Target operand in immediate addressing method is invalid\n", row);
		return -2;
	}
	if(opcode == 6 && (addressS == 0 || addressS == 3))
	{
		printf("Error in line %d source operand in immediate addressing method is invalid\n", row);
		return -2;
	}
	addToListIC();
	tailIcList ->ramWord.opcode = opcode;
	tailIcList ->ramWord.addressS = addressS;
	tailIcList ->ramWord.addressT = addressT;
	tailIcList ->ramWord.are = A;
	tailIcList ->ramWord.wordType = 0;
	memoryCoding(addressS , addressT, rs, rt);
	return 0;
}


void memoryCoding(int addressS , int addressT, char * rs, char * rt)
{
	if(strcmp(rs, "") && addressS == 0)
	{
		addToListIC();	
		tailIcList ->ramWord.number = atoi(rs+1);
		tailIcList ->ramWord.wordType = 1;
		tailIcList ->ramWord.are = A;
	}
	else if(addressS == 1)
	{
		addToListIC();	
		tailIcList ->ramWord.wordType = 1;
        tailIcList ->ramWord.number =0;
	}
	else if(addressS == 2)
	{
		addToListIC();	
		tailIcList ->ramWord.wordType = 1;
        tailIcList ->ramWord.number =0;
		addToListIC();	
		tailIcList ->ramWord.wordType = 1;
		tailIcList ->ramWord.number = (int)(rs[strlen(rs)-1]-48);
		tailIcList ->ramWord.are = A;
	}
	else if(addressS == 3)
	{
		addToListIC();	
		tailIcList ->ramWord.wordType = 2;
		tailIcList ->ramWord.rs = isRegister(rs);
		tailIcList ->ramWord.rt = 0;
		tailIcList ->ramWord.are = A;
	}

	/* Target*/
	if(addressT == 0)
	{
		addToListIC();	
		tailIcList ->ramWord.number = atoi(rt+1);
		tailIcList ->ramWord.wordType = 1;
		tailIcList ->ramWord.are = A;
	}
	if(addressT == 1)
	{
		addToListIC();	
		tailIcList ->ramWord.wordType = 1;
	}
	if(addressT == 2)
	{
		addToListIC();	
		tailIcList ->ramWord.wordType = 1;
		addToListIC();	
		tailIcList ->ramWord.wordType = 1;
		tailIcList ->ramWord.number = (int)(rs[strlen(rs)-1]);
		tailIcList ->ramWord.are = A;
	}
	if(addressT == 3)
	{
		if(addressS == 3)	
			tailIcList ->ramWord.rt = isRegister(rt);
		else
		{
			addToListIC();	
			tailIcList ->ramWord.wordType = 2;
			tailIcList ->ramWord.rt = isRegister(rt);
			tailIcList ->ramWord.rs = 0;
			tailIcList ->ramWord.are = A;
		}
	}
}


int instrucGroup2(int opcode, char *line, int index,int row)
{
	
	char rt[SIZE_STRUCT];
	int i = 0, addressT;
	index = jumpSpace(line,index);
	while(!isspace(line[index]))
	{
		rt[i] = line[index];
		i++;
		index++;
	}
	rt[i] = '\0';
	index = jumpSpace(line,index);
	if(line[index] != '\n')
	{
		printf("in the row %d in the index %d extra text after the operand\n", row, index);
		return -2;
	}
	addressT = address(rt); /* which address is it*/
	if(addressT == -2)
	{
		printf("Error in line %d the operands is invalid\n", row);
		return -2;
	}
	if((opcode == 4 || opcode == 5 || opcode == 7 || opcode == 8 || opcode == 10 || opcode == 11 || opcode == 13) && addressT == 0)		
	{
		printf("Error in line %d Target operand in immediate addressing method is invalid\n", row);
		return -2;
	}
	addToListIC();
	tailIcList ->ramWord.opcode = opcode;
	tailIcList ->ramWord.addressS = 0;
	tailIcList ->ramWord.addressT = addressT;
	tailIcList ->ramWord.are = A;
	tailIcList ->ramWord.wordType = 0;
	memoryCoding( 0 , addressT, "", rt);
	return 1;
}


int instrucGroup3(int opcode, char *line, int index,int row)
{
	index = jumpSpace(line,index);
	if(line[index] != '\n')
	{
		printf("in the row %d in the index %d extra text after the operand\n", row, index);
		return -2;
	}
	addToListIC();
	tailIcList ->ramWord.opcode = opcode;
	tailIcList ->ramWord.addressS = 0;
	tailIcList ->ramWord.addressT = 0;
	tailIcList ->ramWord.are = A;
	tailIcList ->ramWord.wordType = 0;
	return 1;
}


int directiveData(char * line , int index , int row )
{
 	
	int i=0, num , commaFlag=0;
	char number[SIZE_OF_LINE];
	index = jumpSpace(line,index);
	while(line[index] !='\n' )
	{
        i = 0;
		while(line[index] != ','&& !isspace(line[index]))
		{
			number[i] = line[index];
			i++;
			index++; 
		}
		number[i] = '\0';
		index = jumpSpace(line,index);
		if(strcmp(number,""))
			commaFlag=0;
		if(correctDigit(number))
		{
			num=atoi(number);
			addToListDC();
			tailDcList ->ramWord.string = num;
			tailDcList ->ramWord.wordType =3;
			if(line[index] == ',')
			{
				commaFlag = 1;
				index++;
			}
            index = jumpSpace(line,index);
		}
		else 
		{
			printf("in the row %d in the index %d The number is incorrect\n", row, index);
			return -2;
		}
	}
    index = jumpSpace(line,index);
	if(line[index] == '\n' && commaFlag)
	{
		printf("in the row %d in the index %d The syntax of the word is incorrect,there is an unnecessary comma at the end of the sentence\n", row, index);
		return -2;
	}
	return 0;
}

	
int directiveString(char * line , int index , int row )
{
	int start, end;
	index = jumpSpace(line,index);
	start = strchr(line, '"') - line;
	end = strrchr(line, '"')-line;
	if(line[index] != '"' || start == end)
	{
		printf("in the row %d in the index %d The syntax of the string is incorrect\n", row, index);
		return -2;
	}
	index++;
	while(index < end)
	{
		if(!isprint(line[index]))
		{
			printf("in the row %d in the index %d There is a character that cannot be printed\n", row, index);
			return -2;
		}
		addToListDC();
		tailDcList ->ramWord.string = line[index];
		tailDcList ->ramWord.wordType =3;
		index++;
	}
    index++;
	index = jumpSpace(line,index);
	if(line[index]!= '\n')
	{
		printf("in the row %d in the index %d extra text after the end of the string\n", row, index);
		return -2;
	}
	addToListDC();
	tailDcList ->ramWord.string = '\0';
	tailDcList ->ramWord.wordType =3;
	return 0;
}



void addToListDC()
{
	ptDCList new;
	new=(ptDCList) malloc(sizeof(listDC)); /*Allocation of memory*/
	if(!new)
	{ 
		printf("\n cannot allocate memory to a new symbol \n");
	 	exit(0);
	}
	new -> next =NULL;
	if(!headDcList)
	{
		headDcList = new;
		tailDcList = headDcList;	
	}
	else
	{
	tailDcList->next = new;
	tailDcList = new;
	}
	tailDcList-> decimalAddress = DC;
	DC++;
	return;
}



void freeListDC()			/*Memory release*/
{
	ptDCList p;
	while(headDcList)
	{
		p=headDcList;
		headDcList=p->next;
		free(p);
	}
}

void addSymbolIc()
{
    ptSymbol p;
    p = headSymbol;
    while(p)
    {
        if(p-> type == 1)
            p -> location += IC +MEMORY_ACCESS;
        p = p->next;
    }
}

void addIc()
{
	ptDCList p;
	p = headDcList;
	while(p)
	{
		p -> decimalAddress += IC +MEMORY_ACCESS;
		p = p->next;
	}
}

int directiveStruct(char *line ,int index , int row )
{
	int i=0, num , /*commaFlag=0*/start, end;
	char number[SIZE_OF_LINE];
	index = jumpSpace(line,index);
	
		while(line[index] != ','&& !isspace(line[index]))
		{
			number[i] = line[index];
			i++;
			index++; 
		}
		number[i] = '\0';
		index = jumpSpace(line,index);
		if(correctDigit(number))
		{
			num=atoi(number);
			addToListDC();
			tailDcList ->ramWord.string = num;
			tailDcList ->ramWord.wordType =3;
		}
		else 
		{
			printf("in the row %d in the index %d The number is incorrect\n", row, index);
			return -2;
		}
		if(line[index] == ',')
			index++;
		else
		{
			printf("in the row %d in the index %d missing comma in command\n", row, index);
			return -2;
		}
		index = jumpSpace(line,index);
		start = strchr(line, '"') - line;
		end = strrchr(line, '"')-line;
		if(line[index] != '"' || start == end)
		{
			printf("in the row %d in the index %d The syntax of the string is incorrect\n", row, index);
			return -2;
		}
		index++;
		while(index < end)
		{
			if(!isprint(line[index]))
			{
				printf("in the row %d in the index %d There is a character that cannot be printed\n", row, index);
				return -2;
			}
			addToListDC();
			tailDcList ->ramWord.string = line[index];
			tailDcList ->ramWord.wordType =3;
			index++;
		}
        index++;
		index = jumpSpace(line,index);
		if(line[index]!= '\n')
		{
			printf("in the row %d in the index %d extra text after the end of the string\n", row, index);
			return -2;
		}
		addToListDC();
		tailDcList ->ramWord.string = '\0';
		tailDcList ->ramWord.wordType =3;
		return 0;	
		
}
	

int directiveExtern(char *line ,int index ,int row )
{
	char label[SIZE_LABEL_NAME];
	int i=0, j=0;
	index = jumpSpace(line,index);
	if(!isalpha(line[index]))
	{
		printf("error in row %d index  %d: %s - in the  first character in the label no letter was found\n",row, index, line);
		return -2;
	}
	while(!isspace(line[index]))
	{
		label[i] = line[index];
		if(!isalpha(label[i]) && !isdigit(label[i]))
		{
				printf("error in row %d index  %d: %s - non-number and non-letter character was found\n",row, index, label);
				return -2;
		}
		i++;
		index++;
	}
	label[i] = '\0';
	if(isRegister(label) != -1)
	{
		printf("error in row %d index  %d: %s - using language reserved name in label definition\n",row, index, label);
		return -2;
	}
	while(j<5)		{
		if(!strcmp(label ,directives[i]))
		{
			printf("error in row %d index  %d: %s - using language reserved name in label definition\n",row, index, label);
			return -2;
		}
		j++;
	}
	if(isOrder(label) != -1)
	{
		printf("error in row %d index  %d: %s - An attempt to define a label in an assembly language reserved word\n",row, index, label);
		return -2;
	}
	if(sameSymbol(label))
	{
		printf("error in row %d index  %d: %s - Restatement of label in use detected\n",row, index, label);
		return -2;
	}
	index = jumpSpace(line,index);
	if(line[index] != '\n')
	{
		printf("error in row %d index  %d: %s - extra text after the command\n",row, index, label);
		return -2;
	}
	addToSymbolList(label);
    headSymbol -> location = 0;
	headSymbol -> typeE = E;
	return 0;
}

ptSymbol findSymbol(char * label_name)
{
    ptSymbol p = headSymbol;
    while(p != NULL) 					/*As long as p1 is not equal to null*/
    {
        if(!strcmp(label_name ,p -> label_name))
        {
            return p;
        }
        p = p-> next;
    }
    return NULL;
}

int isEntry(char * line , int index , int row)
{
    ptSymbol p;
    char label[SIZE_LABEL_NAME];
    int i = 0;
    index = jumpSpace(line,index);
    while(!isspace(line[index]))
    {
        label[i] = line[index];
        i++;
        index++;
    }
    label[i] = '\0';
    p= findSymbol(label);
    if(!p)
    {
        printf("error in line %d  index  %d: %s - Invalid label\n", row, index, label);
        return -2;
    }
    index = jumpSpace(line,index);
    if(line[index] != '\n')
    {
        printf("error in line %d  index  %d: %s - extra text after the label \n", row, index, label);
        return -2;
    }
    p->typeE = entry;
    return 0;
}

int labelGroup1( char * line,int index,int row)
{
    char rs[SIZE_STRUCT] , rt[SIZE_STRUCT];
    int i = 0, result = 0, addressS, addressT;
    if(ptr)
        ptr = ptr -> next;
    index = jumpSpace(line,index);
    while (line[index] != ',' && !isspace(line[index]))
    {
        rs[i] = line[index];
        i++;
        index++;
    }
    rs[i] = '\0';
    index = jumpSpace(line,index);
        i = 0;
        index++;
        index = jumpSpace(line,index);
        while(!isspace(line[index]) && i< SIZE_STRUCT)
        {
            rt[i] = line[index];
            i++;
            index++;
        }
        rt[i] = '\0';
        index = jumpSpace(line,index);

    addressS = address(rs); /* which address is it*/
    addressT = address(rt); /* which address is it*/
    result = memoryCoding2( addressS,  row , index, rs);
    if(result)
        return -2;
    if(addressS != 3 || addressT != 3 ) {
        result = memoryCoding2(addressT, row, index, rt);
        if (result)
            return -2;
    }
    return 0;
}


int memoryCoding2(int address, int row ,int index, char * label)
{
    ptSymbol p;
    char * pt;
    if(address == 1) /* label*/
    {
        p = findSymbol(label);
        if(!p)
        {
            printf("error in line %d  index  %d: %s - Invalid label\n", row, index, label);
            return -2;
        }

        ptr -> ramWord.number = p -> location;
        if(p ->typeE != E)
            ptr -> ramWord.are=R;
        else {
            ptr->ramWord.are = E;
            addToExternalList(p,ptr ->decimalAddress);
        }
    }
    else if(address == 2) /* struct*/
    {
        pt = strrchr(label , '.');
        *pt ='\0';
        p = findSymbol(label);
        if(!p)
        {
            printf("error in line %d  index  %d: %s - Invalid label\n", row, index, label);
            return -2;
        }
        ptr -> ramWord.number = p -> location;
        if(p ->typeE != E)
            ptr -> ramWord.are=R;
        else {
            addToExternalList( p , ptr->decimalAddress);
            ptr->ramWord.are = E;
        }
            ptr = ptr->next;
    }
    /*else if(address == 0 || address == 3)
        ptr = ptr ->next;*/
    if(ptr)
        ptr = ptr ->next;
    return 0;
}


int labelGroup2( char * line,int index,int row)
{
    char rt[SIZE_STRUCT];
    int i = 0, addressT, result ;
    if(ptr)
        ptr = ptr -> next;
    index = jumpSpace(line,index);
    while(!isspace(line[index]))
    {
        rt[i] = line[index];
        i++;
        index++;
    }
    rt[i] = '\0';
    index = jumpSpace(line,index);
    addressT = address(rt); /* which address is it*/
    result = memoryCoding2( addressT , row, index, rt);
    if( result == -2 )
        return -2;
    return 0;
}


void printSymbol()    /**/
{
    ptSymbol p = headSymbol;
    while(p != NULL) 					/*As long as p1 is not equal to null*/
    {
        printf("%s ,location: %d  ,era: %d \n" , p -> label_name , p -> location, p->typeE);
        p = p-> next;
    }

}

void printIc()    /**/
{
    ptICList p = headIcList;
    while(p != NULL) 					/*As long as p1 is not equal to null*/
    {
        if(p -> ramWord. wordType == 0)
            printf("opcode: %d ,addressS: %d , addressT: %d ,are: %d  \n " , p -> ramWord . opcode , p -> ramWord.addressS ,p ->ramWord.addressT, p->ramWord.are);
        if(p -> ramWord. wordType == 1)
            printf("number: %d ,are: %d  " , p -> ramWord . number , p -> ramWord.are);
        if(p -> ramWord. wordType == 2)
            printf("rs: %d ,rt: %d , are: %d    " , p -> ramWord .rs , p -> ramWord.rt , p->ramWord.are);
        if(p -> ramWord. wordType == 3)
            printf("string: %d   " , p -> ramWord .string );
        printf("decimal location: %d  \n", p->decimalAddress);
        p = p-> next;
    }

}















	












