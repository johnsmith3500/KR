
/* 
 * Write a private version of scanf analogous to minprintf from the previous section.
 *
 * ex74.c - programm for exercise 7.4 from K&R 
 * http://www.interpork.com
 */

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 100
char buf[BUFSIZE];  // input buffer
int bufp=0;         // current number of chars in buffer

void minscanf(char *fmt, ...);
int mygetint(void);
int getch(void);
void ungetch(int c);
float mygetfloat(void);
void mygetstr(char s[]);

int main (int argc, const char * argv[])
{
    char s1[100];
    int i1;
    float f1;
    
    printf("enter a float, a string and an integer:\n");
    minscanf("%f%s%d", &f1, s1, &i1);
    printf("minscanf: \nf1: %f\ns1: %s\ni1: %d\n", f1, s1, i1);
    
    return 0;
}

void mygetstr(char tempstr[])   // function return string from stdin
{
    int i=0, c;
    
    while(isspace(tempstr[0]=c=getch()))    // skip white space in input
        ;
    while(!isspace(tempstr[++i]=c=getch()))
        ;
    tempstr[i]='\0';
    if(c != EOF)
        ungetch(c); // return last char (white space) to buffer
    return;
}

float mygetfloat(void)  // function return float from stdin
{
    char tempstr[1000];
    int i=0, c;
    
    // skip white space in input
    while((tempstr[0]=c=getch()) == ' ' || c == '\t' || c == '\n')
        ;
    if(isdigit(c))  // get digits before point
        while(isdigit(tempstr[++i]=c=getch()))
            ;
    if(c == '.')
        while(isdigit(tempstr[++i]=c=getch()))  // get digits after point
            ;
    tempstr[i]='\0';
    if(c != EOF)
        ungetch(c);     // return last char (not digit) to buffer
    return atof(tempstr);
}
    

int getch(void)     // get char from buffer if it is not empty otherwise get char from stdin
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) // return char into buffer
{
    if(bufp >= BUFSIZE)
        printf("ungetch: too many symbols\n");
    else
        buf[bufp++]=c;
}

int mygetint(void)  // function return integer from stdin
{
    int retval, i=0;
    char tempstr[1000], c;
    
    // skip white space in input
    while((tempstr[0]=c=getch()) == ' ' || c == '\t' || c == '\n')
        ;
    tempstr[1]='\0';
    i++;
    while(isdigit(tempstr[i]=c=getch()))    // get all digits from input
    {
        i++;
    }
    tempstr[i]='\0';
    retval=atoi(tempstr);
    ungetch(c);         // return last char (not digit) to buffer
    return retval;
}

void minscanf(char *fmt, ...)
{
    va_list ap;
    char *p, sval[1000], *sptr;
    int ival, myerror=0;
    int *iptr;
    float fval, *fptr;
    
    va_start(ap, fmt);
    for(p=fmt; *p; p++)
    {
        if(*p != '%')
            continue;
        switch(*++p)
        {
            case 'd':   // we have integer in format string
                ival=mygetint();
                iptr=va_arg(ap, int *); // iptr now point to argument
                *iptr=ival;     // write integer from stdin to argument
                break;
            case 'f':   // we have float in format string
                fval=mygetfloat();
                fptr=va_arg(ap, float *);   // fptr now point to argument
                *fptr=fval;     // write float from stdin to argument
                break;
            case 's':   // we have string in format string
                mygetstr(sval);
                sptr=va_arg(ap, char *);    // sptr now point to argument
                strcpy(sptr, sval);     // write string from stdin to argument
                break;
            default:
                printf("[ERROR]: wrong parametr\n");
                myerror=1;
                break;
        }
        if(myerror)
            break;
    }
    va_end(ap);
}