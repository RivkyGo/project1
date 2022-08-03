#include "convertText.h"

char base[]={'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
extern ptICList headIcList;
extern ptDCList  headDcList;
extern ptExtern headExternList;
extern IC , DC;
extern int entryFlag;
extern int externFlag;
extern ptSymbol headSymbol;
char numInBase[3];
/*extern enum ARE {A, E, R , entry};*/
void ConvertToBase(int num)
{
    int firstNum, secondNum ;
    secondNum = num%SIZE_BASE;
    num = num/SIZE_BASE ;
    firstNum = num%SIZE_BASE;
    numInBase[0] = base[firstNum];
    numInBase[1] = base[secondNum];
    numInBase[2] = '\0';
}

int convertToObject(char * file_name)
{
    ptICList ptIc = headIcList;
    ptDCList ptDc = headDcList;
    FILE * file;
    int error , number , mask = 1, temp;
    char * fullFileName;
    fullFileName = concat(file_name, ".ob");
    file = fopen(fullFileName, "w");
    if(!file)
    {
        printf("can not open %s file",fullFileName);
        error = 1;
    }
    ConvertToBase(IC );
    fputs(numInBase, file);
    fputs(" ", file);
    ConvertToBase(DC );
    fputs(numInBase, file);
    fputs("\n", file);
    while(ptIc)
    {
        ConvertToBase( ptIc ->decimalAddress);
        fputs(numInBase, file);
        fputc('\t', file);
        if(ptIc ->ramWord.wordType ==0)
        {
            number= (int)ptIc ->ramWord.opcode;
            mask=1;
            number = number<<2;
            temp=(int)ptIc ->ramWord.addressS;
            number= number|(mask&temp);
            mask=mask<<1;
            number= number|(mask&temp);
            mask=1;
            number = number<<2;
            temp=(int)ptIc ->ramWord.addressT;
            number= number|(mask&temp);
            mask=mask<<1;
            number= number|(mask&temp);
            mask=1;
            number = number<<2;
            temp=(int)ptIc ->ramWord.are;
            number= number|(mask&temp);
            mask=mask<<1;
            number= number|(mask&temp);
            mask=1; /* type 0 */
        }
        else if(ptIc ->ramWord .wordType ==1)  /* number*/
        {
            number= (int)ptIc ->ramWord.number;
            mask=1;
            number = number<<2;
            temp=(int)ptIc ->ramWord.are;
            number= number|(mask&temp);
            mask=mask<<1;
            number= number|(mask&temp);  /* type 1 */
        }
        else if(ptIc ->ramWord .wordType ==2)
        {
            number= (int)ptIc ->ramWord.rs;
            number = number<<4;
            mask = 1;
            temp=(int)ptIc ->ramWord.rt;
            number= number|(mask&temp);
            mask=mask<<1;
            number= number|(mask&temp);
            mask=mask<<1;
            number= number|(mask&temp);
            mask=mask<<1;
            number= number|(mask&temp);
            number = number<<2;
            mask = 1;
            temp=(int)ptIc ->ramWord.are;
            number= number|(mask&temp);
            mask=mask<<1;
            number= number|(mask&temp);
            /* type 2 */
        }
        else
            number= (int)ptIc ->ramWord.string;      /* type 3 */
        ConvertToBase(number);
        fputs(numInBase, file);
        fputc('\n', file);
        ptIc = ptIc -> next;
    }
    while(ptDc)
    {
        ConvertToBase( ptDc ->decimalAddress);
        fputs(numInBase, file);
        fputc('\t', file);
        number= (int)ptDc ->ramWord.string;      /* type 3 */
        ConvertToBase(number);
        fputs(numInBase, file);
        fputc('\n', file);
        ptDc = ptDc -> next;
    }

    fclose(file);
    free(fullFileName);
    return 0;
}







char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    if(result)
    {
        strcpy(result, s1);
        strcat(result, s2);
        return result;
    }
    else
    {
        return NULL;
    }
}


void convertToEntry(char * file_name)
{
    ptSymbol p = headSymbol;
    FILE * file;
    char * fullFileName;
    fullFileName = concat(file_name, ".ent");
    file = fopen(fullFileName, "w");
    if(!file)
    {
        printf("can not open %s file",fullFileName);
        return;
    }
    while(p)
    {
        if(p ->typeE ==  3)
        {
            fputs(p ->label_name, file);
            ConvertToBase( p ->location);
            fputc('\t', file);
            fputs(numInBase, file);
            fputc('\n', file);
        }
        if(p)
            p = p->next;
    }
    fclose(file);
    free(fullFileName);
}

void convertToExtern(char * file_name)
{
    ptExtern p = headExternList;
    FILE * file;
    char * fullFileName;
    fullFileName = concat(file_name, ".ext");
    file = fopen(fullFileName, "w");
    if(!file)
    {
        printf("can not open %s file",fullFileName);
        return;
    }
    while(p)
    {
        fputs(p ->label_name, file);
        ConvertToBase( p ->location);
        fputc('\t', file);
        fputs(numInBase, file);
        fputc('\n', file);
        if(p)
            p = p->next;
    }
    fclose(file);
    free(fullFileName);
}