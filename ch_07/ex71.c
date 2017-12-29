/* 
 * ex71.c ; exercise 7.1 from k&r; 
 * compiling: 1.  cc ex71.c -o mytolower2; 2. cc ex71.c -o mytoupper2  
 *  
 * Write a program that converts upper case to lower or lower case to upper, 
 * depending on the name it is invoked with, as found in argv[0].
 *
 * http://www.interpork.com
 * 
 * For additional information see K&R paragraph 7.1
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void mytolower2(void);
void mytoupper2(void);

int main (int argc, const char * argv[])
{
    char name[100];
    
    strcpy(name, strrchr(argv[0], '/')+1);  // take program's name to 'name' variable
    if(strcmp(name, "mytolower2")==0)
        mytolower2();
    else if(strcmp(name, "mytoupper2")==0)
        mytoupper2();
    else
        printf("[ERROR] wrong name!\n");
        
    return 0;
}

void mytoupper2(void)
{
    int c;
    
    while((c=getchar())!=EOF)   // read data from stdin
        putchar(toupper(c));    // write data to stdout
    
    return;
}

void mytolower2(void)
{
    int c;
    
    while((c=getchar())!=EOF)
        putchar(tolower(c));
    
    return;
}