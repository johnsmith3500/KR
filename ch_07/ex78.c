/* 
 * Write a program to print a set of files, 
 * starting each new one on a new page, 
 * with a title and a running page count for each file
 * 
 * ex78.c - program 7.8 from k&r book 
 * http://www.interpork.com
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define ROWS 26 // number of lines in one page

int main (int argc, const char * argv[])
{
    FILE *fd;
    int c, lines, whole_pages, blank_lines, i, page;
    
    printf("\e[2J\e[H");    // clear console screen
    if(argc==1)     // no input files
    {
        while((c=getc(stdin))!=EOF)
            putc(c, stdout);
    }
    else
    {
        while(--argc>0)     // while we have files to process
        {
            fd=fopen(*++argv, "r");     // open next file for reading
            if(fd==NULL)            // fail to open
            {
                fprintf(stderr, "%s\n", "[ERROR] cannot open file");
                exit(1);
            }
            else                // success to open
            {
                lines=0;
                page=1;
                // delimeter between files
                printf("-------------------------------%s-----------------------------\n", *argv);
                // delimeter between pages of a file
                printf("~~~page %d~~~\n", page);
                while((c=getc(fd))!=EOF)
                {
                    putc(c, stdout);
                    if(c=='\n')
                    {
                        lines++;
                        if((lines%ROWS)==0) // get current line on current page, if the line is last on page, then print page-delimeter
                            printf("~~~page %d~~~\n", ++page);
                    }

                }
                whole_pages=lines/ROWS;
                blank_lines=(whole_pages+1)*ROWS-lines;
                for(i=0; i<blank_lines; i++)
                    printf("\n");
/*                printf("lines=%d\n", lines);
                printf("whole pages=%d\n", whole_pages);
                printf("blank_lines=%d\n", blank_lines);
*/
                fclose(fd);
            }            
        }
    }

    if(ferror(stdout))
        fprintf(stderr, "%s\n", "[ERROR] cannot make output");
    
    return 0;
}


