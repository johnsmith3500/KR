/* 
 * Modify getop so that it doesn't need to use ungetch. 
 * Hint: use an internal static variable.
 *
 * ex411.c - program 4.11 from k&r book; 
 * http://www.interpork.com
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP 100   // max size of operand or operator
#define NUMBER '0'  // we have a number
#define MAXVAL 100  // max depth of the stack
#define BUFSIZE 100 // buffer for ungetch
int sp=0;           // next free position in the stack
double val[MAXVAL]; // stack
char buf[BUFSIZE];  // buffer
int bufp=0;         // position in buffer

int getop(char []);     // get next operand or operator
void push(double);      // put value of f into the stack
double pop(void);       // get value from top of the stack and return it
int getch(void);        // get (probably from buffer) char

int main(int argc, const char * argv[])
{
    int type;   // hold operator or NUMBER (if we have a number)
    double op2; // aux var for temporarily hold a number
    char s[MAXOP];  // hold user's string
    
    while((type=getop(s))!=EOF)
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
    
    return 0;
}

void push(double f) // put value of f into the stack
{
    if(sp<MAXVAL)
        val[sp++]=f;
    else
        printf("error: stack full, %g cannot be hold\n", f);
}

double pop(void)    // get value from top of the stack and return it
{
    if(sp>0)
        return val[--sp];
    else
    {
        printf("error: stack is empty\n");
        return 0.0;
    }
}

int getop(char s[])     // get next operand or operator
{
    static int c=-1;
    int i, temp;
    if(c<0)
        c=getch();
    while(c==' ' || c=='\t')    // skip white space
        c=getch();
    s[0]=c;
    s[1]='\0';
    if(!isdigit(c) && c!='.')   // not number
    {
        temp=c;
        c=-1;
        return temp;
    }
    i=0;
    if(isdigit(c))      // collect integer part
        while(isdigit(s[++i]=c=getch()))
            ;
    if(c=='.')          // collect exponent part
        while(isdigit(s[++i]=c=getch()))
            ;
    s[i]='\0';
  
    return NUMBER;
}

int getch(void)     // get (probably from buffer) char
{
    return (bufp>0) ? buf[--bufp] : getchar();
}

