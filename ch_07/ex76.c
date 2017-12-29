/* 
 * Write a program to compare two files, printing the first line where they differ.
 *
 * ex76.c - programm for exercise 7.5 from K&R 
 * http://www.interpork.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 4096

int main(int argc, char *argv[])
{
    FILE *fp1, *fp2;
    char buf1[MAXLINE], buf2[MAXLINE];

/* check usage */
    if(argc!=3)
    {
        printf("wrong usage!\n");
        exit(1);
    }
/* open files for reading */
    if((fp1=fopen(argv[1], "r"))==NULL)
    {
        perror("read()");
        exit(1);
    }
    if((fp2=fopen(argv[2], "r"))==NULL)
    {
        perror("read()");
        exit(1);
    }
    while(1)
    {
/* get strings from files and check for errors */
        fgets(buf1, MAXLINE, fp1);
        if(ferror(fp1))
        {
            perror("fgets()");
            exit(1);
        }
        fgets(buf2, MAXLINE, fp2);
        if(ferror(fp2))
        {
            perror("fgets()");
            exit(1);
        }
/* compare two strings; if they differ then print them and exit */
        if(strcmp(buf1, buf2)!=0)
        {
            printf("%s", buf1);
            printf("%s", buf2);
            break;
        }
/* check for end of the files */
        if(feof(fp1) && feof(fp2))
            break;
    }
    fclose(fp1);
    fclose(fp2);

    return 0;
}
