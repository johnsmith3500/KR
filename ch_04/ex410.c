/* 
 * An alternate organization uses getline to read an entire input line; 
 * this makes getch and ungetch unnecessary. 
 * Revise the calculator to use this approach.
 *
 * ex410.c - program 4.10 from k&r book; 
 * http://www.interpork.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP 100       // max size of operand or operator
#define NUMBER '0'      // we have a number
#define MAXLINE 1000    // max length of input string
#define MAXVAL 100      // max depth of the stack
#define BUFSIZE 100     // buffer for ungetch
int getop(char s[], char line[]);   // get next operand or operator
void push(double);      // put value of f into the stack
double pop(void);       // get value from top of the stack and return it
int mygetline(char line[], int lim);    // get string from stdin
int lp=0;       // position in user's line
int sp=0;       // next free position in the stack
double val[MAXVAL];     // stack

int main(int argc, const char * argv[])
{
    int type, len;
    double op2;
    char s[MAXOP], line[MAXLINE];
    
    while((len=mygetline(line, MAXLINE))>0) // get next line from stdin
    {
        lp=0;
        while((type=getop(s, line))!='\0')
        {
            switch(type)
            {
            case NUMBER:
                push(atof(s));
                break;
            case '+':
                push(pop()+pop());
                break;
            case '*':
                push(pop()*pop());
                break;
            case '-':
                op2=pop();
                push(pop()-op2);
                break;
            case '/':
                op2=pop();
                if(op2!=0.0)
                    push(pop()/op2);
                else
                    printf("error: dividing by zero\n");
                break;
            case '\n':
                printf("\t%.8g\n", pop());
                break;
            default:
                printf("error: unknown operation %s\n", s);
                break;
            }
        }
    }
    
    return 0;
}

int mygetline(char s[], int lim)    // get string from stdin
{
    int c, i;
    for(i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
        s[i]=c;
    if(c=='\n')
    {
        s[i]=c;
        ++i;
    }
    s[i]='\0';
    return i;
}

// put value of f into the stack
void push(double f)
{
    if(sp<MAXVAL)
        val[sp++]=f;
    else
        printf("error: stack full, %g cannot be hold\n", f);
}

// get value from top of the stack and return it
double pop(void)
{
    if(sp>0)
        return val[--sp];
    else
    {
        printf("error: stack is empty\n");
        return 0.0;
    }
}

// get next operand or operator
int getop(char s[], char line[])
{
    int i, temp;
    
    while(line[lp]==' ' || line[lp]=='\t')
        lp++;
    if(!isdigit(line[lp]) && line[lp]!='.') // not number
    {
        temp=lp;
        lp++;
        return line[temp];
    }
    i=0;
    while(isdigit(line[lp]))    // collect integer part
    {
        s[i]=line[lp];
        i++;
        lp++;
    }
    if(line[lp]=='.')
    {
        s[i]=line[lp];
        i++;
        lp++;
        while(isdigit(line[lp]))    // collect exponent part
        {
            s[i]=line[lp];
            i++;
            lp++;
        }
    }
    s[i]='\0';
    
    return NUMBER;
}
