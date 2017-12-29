/*
 * Write a program that will print arbitrary input in a sensible way. 
 * As a minimum, it should print non-graphic characters in octal or 
 * hexadecimal according to local custom, and break long text lines.
 *
 * ex72.c (version 1.0): make strange symbols visible; exercise 7.2
 * Spaces at the end of line are marked; tabulation print as \t, new line as \n etc or as \0xx
 * Program asks user about string's length, for convinient reading; NO, THIS VERSION USES 
 * HARD CODED len VARIABLE (YOU CAN CHANGE) 
 * Data is coming from stdin; for example (unix/linux): ./e7_2<abc.txt 
 *
 * http://www.interpork.com
 *
 * For additional information see K&R paragraph 7.2
 */
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
void printspace(int sp);    /* print space as regular space: ' ' */
void prints(int sp);        /* instead space print \s */
int n;        /* current number of char in string */
int len;    /* length of string, border for convinient reading */
int main(int argc, char *argv[])
{
    int c;        /* current char */
    int needsp=0;    /* buffer for spaces: hold number of spaces in a row */
        
/*    printf("enter length of string (0 - don't break strings): ");
    scanf("%d%c", &len, &ch); NO WORKING THEN DATA ALSO COMING FROM    STDIN */
    len=60;
    n=0;
    while((c=getchar())!=EOF)
    {
        if(isascii(c) && isprint(c) && c!=' ')
        {
            printspace(needsp);
            needsp=0;
            putchar(c);
        }
        else if(c=='\n')
        {
            prints(needsp);
            needsp=0;
            printf("\\n");
        }
        else if(c=='\t')
        {
            printspace(needsp);
            needsp=0;
            printf("\\t");
        }
        else if(c=='\a')
        {
            printspace(needsp);
            needsp=0;
            printf("\\a");
        }
        else if(c=='\b')
        {
            printspace(needsp);
            needsp=0;
            printf("\\b");
        }
        else if(c=='\f')
        {
            printspace(needsp);
            needsp=0;
            printf("\\f");
        }
        else if(c=='\r')
        {
            printspace(needsp);
            needsp=0;
            printf("\\r");
        }
        else if(c=='\v')
        {
            printspace(needsp);
            needsp=0;
            printf("\\v");
        }
        else if(c==' ')
            needsp++;
        else
        {
            printspace(needsp);
            needsp=0;
            printf("\\%03o", c);
        }
        if(c!=' ')
            n++;
        if(n==len)
        {
            printf("\n");
            n=0;
        }            
    }
    prints(needsp);
    printf("\n");
    return 0;
}

void prints(int sp)
{
    int i;
    
    for(i=0; i<sp; i++)
    {
        printf("\\s");
        n++;
        if(n==len)
        {
            printf("\n");
            n=0;
        }
    }
        
}

void printspace(int sp)
{
    int i;
    
    for(i=0; i<sp; i++)
    {
        printf(" ");
        n++;
        if(n==len)
        {
            printf("\n");
            n=0;
        }
    }
}
