/* 
 * Rewrite the program cat from Chapter 7 using read , write , open and close 
 * instead of their standard library equivalents. 
 * Perform experiments to determine the relative speeds of the two versions.
 *
 * ex81.c - program 8.1 from k&r book; tested on freebsd 10.3 
 * http://www.interpork.com
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFS 64     // size of buffer for copying

int mygetc_buf(int fd);
int myputc(int c, int fd);
void filecopy(int ifd, int ofd);

int main (int argc, const char * argv[])
{
    int fd, i;
    const char *prog=argv[0];
    
    if(argc==1) // if no input files - concatenate from stdin to stdout
        filecopy(0, 1);
    else
        while(--argc>0)
        {
            if((fd=open(*++argv, O_RDONLY, 0))==-1) // open file, check for errors
            {
                fprintf(stderr, "%s: [ERROR] cannot open file %s\n", prog, *argv);
                exit(1);
            }
            else
            {
                filecopy(fd, 1);    // cope from file to stdout
                close(fd);    
            }
        }
    if(ferror(stdout))
    {
        fprintf(stderr, "%s: [ERROR] error writing into stdout\n", prog);
        exit(2);
    }
    
    return 0;
}

void filecopy(int ifd, int ofd) // copy chars from file descriptor 'ifd' to 'ofd' 
{
    int c;
    
    while((c=mygetc_buf(ifd))!=EOF)
        myputc(c, ofd);
}

int myputc(int c, int fd)   // write char c to file fd, return written char or eof
{
    unsigned char b=(unsigned char)c;
    
    return (write(fd, &b, 1)==1) ? b : EOF;
}

// buffered input: put BUFS number of chars into buf and return them by one
int mygetc_buf(int fd)
{
    static char buf[BUFS];
    static char *bufp=buf;
    static int n=0;
    
    if(n<=0)    // buffer empty?
    {
        n=read(fd, buf, sizeof buf);    // fill the buffer
        bufp=buf;                       // set bufp to beginnig of the buffer
    }
    
    return (--n>=0) ? (unsigned char)*bufp++ : EOF; // return next char if any
}








