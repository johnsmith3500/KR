/* 
 * Revise minprintf to handle more of the facilities of printf .
 *
 * exercise 7.3 from k&r;
 * This minprintf can handle: minus sign for to the left or to the right justification; 
 * width of field. 
 *
 * http://www.interpork.com
 *
 * For additional information see K&R paragraph 7.3
 */
                        
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>

void minprintf(char *fmt, ...);
int getcharsofint(int d);

int main (int argc, const char * argv[])
{
    int i=123;
    double db=456.789;
    char *s="abcdef";
    
    minprintf("%-7d%-7d%-7d\n", i, i, i);
    minprintf("%7d%7d%7d\n", i, i, i);
    minprintf("%-15f%-4d\n", db, i);
    minprintf("%-10s%-4d\n", s, i);
    minprintf("%10s%4d\n", s, i);
    minprintf("%-10d%-10d\n%-10d%-10d\n", 123, 5, 27, 6789);
    
    return 0;
}

// get number of digits (minus sign including) in an integer
int getcharsofint(int d)
{
    int retval=1;
    
    if(d<0)
        retval++;
    while((d/=10) != 0)
        retval++;
    return retval;
}

void minprintf(char *fmt, ...)
{
    va_list ap;
    char *p;    // current char in format string
    char *sval;
    char fieldstring[80];   // array for holding digits of width of field (width as string)
    char tempstring[80];
    int field;      // width of field as integer
    int fsindex;    // number of digits in width of the field
    int fieldmargin;    // size of empty space in field (in number of symbols)
    int ival;       // store integer value here
    int i;
    int tempstrindex;   // current number of chars in the string
    int totheleft=0;    // move text to the left side of the field
    double dval;    // store float value here
    
    va_start(ap, fmt);
    for(p=fmt; *p; p++)
    {
        // we have regular char - nothing need to do - just print it and go next char
        if(*p!='%')
        {
            putchar(*p);
            continue;
        }
        // we have '-'(left justification), so remember it and go next char
        if(*++p =='-')
        {
            totheleft=1;
            ++p;
        }
        // zeroing field aux vars
        field=0;
        fsindex=0;
        // if *p is digit thus we have width of field
        while(isdigit(*p))
        {
            fieldstring[fsindex++]=*p;
            ++p;
        }
        // convert width of field from string to integer
        if(fsindex)
        {
            fieldstring[fsindex]='\0';
            field=atoi(fieldstring);
        }
        switch(*p)
        {
            case 'd':   // we have integer
                ival=va_arg(ap, int);
                fieldmargin=field-getcharsofint(ival);
                if(totheleft)
                    printf("%d", ival);
                for(i=0; i<fieldmargin; i++)
                    putchar(' ');
                if(!totheleft)
                    printf("%d", ival);
                break;
            case 'f':   // we have float
                dval=va_arg(ap, double);
                fieldmargin=field-(getcharsofint((int)dval)+7);
                if(totheleft)
                    printf("%f", dval);
                for(i=0; i<fieldmargin; i++)
                    putchar(' ');
                if(!totheleft)
                    printf("%f", dval);
                break;
            case 's':   // we have string
                tempstrindex=0;
                for(sval=va_arg(ap, char *); *sval; sval++)
                    tempstring[tempstrindex++]=*sval;
                tempstring[tempstrindex]='\0';
                fieldmargin=field-tempstrindex;
                if(totheleft)
                    printf("%s", tempstring);
                for(i=0; i<fieldmargin; i++)
                    putchar(' ');
                if(!totheleft)
                    printf("%s", tempstring);
                break;
            default:    // unknown specifier; print it as-is
                putchar(*p);
                break;
        }
    }
    va_end(ap);
}
