/*
 * makeinvis.c - program make a text file filled with invisible chars.
 * Invisible chars have code from 0 to 32 (decimal).
 * Output is in strange.txt
 *
 * http://www.interpork.com
 */
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd, code=0;
    
    if((fd=open("strange.txt", O_CREAT | O_WRONLY | O_TRUNC, 0777))<0)
    {
        perror("open()");
        exit(1);
    }
    while(code!=(-1))
    {
        printf("enter code of char from 0 to 255 (-1 for exit)\n");
        printf("unvisible chars have code from 0 to 32 (decimal)\n");
        scanf("%d", &code);
        if(code > 255 || code < (-1))   // validate input
        {
            printf("wrong code!\n");
            continue;
        }
        if(code>=0)
        {
            if(write(fd, &code, 1)!=1)  // write byte into file
            {
                perror("write()");
                exit(1);
            }
        }
    }
    printf("Your chars are in strange.txt file\n");

    return 0;
}