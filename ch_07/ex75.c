
/* 
 * Rewrite the postfix calculator of Chapter 4 to use scanf and/or sscanf 
 * to do the input and number conversion.
 *
 * ex75.c - programm for exercise 7.5 from K&R 
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
int sp=0;               // next free position in the stack
double val[MAXVAL];     // stack
char buf[BUFSIZE];      // next free position in the buf
int bufp=0;

int getop(char []);
void push(double);
double pop(void);
int mygetline(char s[], int lim);
int getch(void);
void ungetch(int);

int main(int argc, const char * argv[])
{
    int type, len, end, i, n;
    float op2, fval;
    char s[MAXOP], line[MAXLINE], tempstr[3];
    
    while((len=mygetline(line, MAXLINE))>0) // get next line from stdin
    {
        end=0;
        i=0;
        n=0;
        while(!end)
        {
            if(sscanf(line+i, "%f%n", &fval, &n)==1)    // get operand
            {
                push(fval);
                i+=n;
            }
            else if(sscanf(line+i, "%s%n", tempstr, &n)==1) // get operator
            {
                tempstr[1]='\0';
                switch(tempstr[0])
                {
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
                    
                    default:
                        printf("[ERROR]: unknown operation %s\n", tempstr);
                        break;
                }
                i+=n;
            }
            else
            {
                end=1;
                printf("\t%.8g\n", pop());
            }
            
        }
    }    
    
    
/*    
    
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
*/    
    return 0;
}

// get string from stdin
int mygetline(char s[], int lim)
{
    int c, i;
    
    for(i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; i++)
        s[i]=c;
    if(c=='\n')
    {
        s[i]=c;
        i++;
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
int getop(char s[])
{
    int i,c;
    while((s[0]=c=getch())==' ' || c=='\t')
        ;
    s[1]='\0';
    if(!isdigit(c) && c!='.')
        return c;   // not number
    i=0;
    if(isdigit(c))  // collect integer part
        while(isdigit(s[++i]=c=getch()))
            ;
    if(c=='.')      // collect exponent part
        while(isdigit(s[++i]=c=getch()))
            ;
    s[i]='\0';
    if(c!=EOF)
        ungetch(c);
    return NUMBER;
}

// get (probably from buffer) char
int getch(void)
{
    return (bufp>0) ? buf[--bufp] : getchar();
}

// put char back to input
void ungetch(int c)
{
    if(bufp>=BUFSIZE)
        printf("ungetch: too many symbols\n");
    else
        buf[bufp++]=c;
}

