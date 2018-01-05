/* 
 * Extend atof to handle scientific notation of the form 123.45e-6 
 * where a floating-point number may be followed by e or E and 
 * an optionally signed exponent.
 *
 * ex42.c - program 4.2 from k&r book; 
 * http://www.interpork.com
 */

#include <stdio.h>
#include <ctype.h>

double my_atof (char s[]);

int main(int argc, const char * argv[])
{
    char s[100];       // holds user's input
    double dd;          // user's input converted to double
    
    printf("enter double: ");
    scanf("%s", s);
    dd=my_atof(s);
    printf("double from string: %.9f\n", dd);
    printf("double from string: %e\n", dd);    
    
    return 0;
}

double my_atof (char s[])    // convert string to double
{
    double val, power, retval;
    int i, sign, j, mantisa, mantisasign;
    
    for(i=0; isspace(s[i]); i++)    // skip spaces in the string
        ;
    sign=(s[i]=='-')? -1 : 1;       // determine sign of the number
    if(s[i]=='+' || s[i]=='-')      // skip sign if any
        i++;
    for(val=0.0; isdigit(s[i]); i++)    // get numbers before point
        val=10.0*val+(s[i]-'0');
    if(s[i]=='.')   // skip point
        i++;
    for(power=1.0; isdigit(s[i]); i++)  // get numbers after point and before e(E)
    {
        val=10.0*val+(s[i]-'0');
        power*=10.0;        // power/10 - number of digits after point
    }
    
    retval=sign*val/power;  // our double like 123.45678
    
    if(s[i]=='e' || s[i]=='E')  // we have e(E) in the double
    {
        i++;
        mantisasign=(s[i]=='-')?-1:1;   // get sign of the mantisa
        if(s[i]=='+' || s[i]=='-')      // get after sign
            i++;
        for(mantisa=0; isdigit(s[i]); i++)  // get numbers after mantisa
            mantisa=10*mantisa+(s[i]-'0');
        if(mantisasign==-1)             // if mantisa is negative - divide
           for(j=1; j<=mantisa; j++)
               retval/=10.0;
        else                            // if mantisa is positive - multiply
            for(j=1; j<=mantisa; j++)
                retval*=10.0;
    }
    
    return retval;
           
}

