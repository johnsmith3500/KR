/* 
 * Modify the pattern finding program of Chapter 5 to take its input 
 * from a set of named files or, if no files are named as arguments, 
 * from the standard input. 
 * Should the file name be printed when a matching line is found?
 *
 * ex77.c - program for exercise 7.7 from k&r book 
 * usage in nix: ./e7_7 abc file1.txt file2.txt ... 
 * search substring abc in the files 
 * http://www.interpork.com
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 1000

int mystrindex(char s[], char sf[]);
int mystrfile(FILE *fp, char sf[]);

int main(int argc, const char * argv[])
{
    FILE *fp;
    char searchfor[MAX];    // string we need to find
    
    if(argc==1)     // check command line arguments
    {
        printf("[ERROR] wrong parameters\n");
        exit(1);
    }
    strcpy(searchfor, *++argv); // searchfor now contains what we need to find
    if(argc==2)     // search in stdin
    {
        printf("!!! stdin: !!!\n");
        mystrfile(stdin, searchfor);
        printf("*******************************\n");
    }
    else            // search in provided file(s)
    {
        while(--argc>1)
        {
            ++argv;
            printf("!!! %s: !!!\n", *argv);
            if((fp=fopen(*argv, "r"))==NULL)
            {
                fprintf(stderr, "cannot open file %s\n", *argv);
                exit(1);
            }
            else
            {
                mystrfile(fp, searchfor);
                printf("*******************************\n");
            }
        }
    }
        

    
    return 0;
}

int mystrfile(FILE *fp, char sf[])  // find string sf in file fp
{
    int retval=0;
    char temp[MAX];
    
    while(fgets(temp, MAX, fp))     // go through all lines
    {
        if(mystrindex(temp, sf)>=0) // we have the string - print it
        {
            printf("%s", temp);
            retval++;
        }
    }
    printf("found: %d\n", retval);
    return retval;
}

int mystrindex(char source[], char searchfor[]) // find string 'searchfor' in string 'source'
{
    int i, j, tempi, slen, srlen;
    slen=strlen(source);
    srlen=strlen(searchfor);
    for(i=0; i<slen; i++)   // go through all chars of 'source'
    {
        j=0;
        if(source[i]==searchfor[j])     // we find first char of 'searchfor' in 'source',
        {                               // so we need to check further
            tempi=i+1;
            j++;
            while(source[tempi]==searchfor[j])
            {
                tempi++;
                j++;
            }
            if(j>=srlen)
                return i;       // Match! Return index of the first matching chars.
            
        }
    }
    return -1;      // No match...
}
