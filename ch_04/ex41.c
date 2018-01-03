/* 
 * Write the function strrindex(s,t) , 
 * which returns the position of the rightmost occurrence of t in s , 
 * or -1 if there is none.
 *
 * ex41.c - program 4.1 from k&r book; 
 * http://www.interpork.com
 */

#include <stdio.h>
#define MAXLINE 1000    // maximum length of the input string

int mygetline(char line[], int max);    // get line from stdin
int strindex(char source[], char searchfor[]);  // get index of last occurrence of "searchfor" in "source"
char pattern[]="ould";  // what we are looking for

int main(int argc, const char * argv[])
{
    char line[MAXLINE];     // holds line from stdin
    int found=0;
    int index;
    
    while(mygetline(line, MAXLINE)>0)   // while there is line in stdin
    {
        index=strindex(line, pattern);  // check for pattern
        if(index>=0)    // match!
        {
            printf("index=%d\n", index);
            found++;
        }
        else
        {
            printf("No found.\n");
        }
    }
    return found;
}
int mygetline(char s[], int lim)    // get line from stdin
{
    int c, i;
    i=0;
    while(--lim>0 && (c=getchar())!=EOF && c!='\n')
        s[i++]=c;
    if(c=='\n')
        s[i++]=c;
    s[i]='\0';
    return i;
}

int strindex(char s[], char t[])    // get index of last occurrence of "t" in "s"
{
    int i, j, rp;
    int k;      // number of chars already match
    rp=-1;              // current index of occurrence
    
    for(i=0; s[i]!='\0'; i++)   // go through all chars of s
    {
        for(j=i, k=0; t[k]!='\0' && s[j]==t[k]; j++, k++)   // check all chars in t 
            ;
        if(k>0 && t[k]=='\0')   // whole t is in s
            rp=i;
    }
    return rp;
}
