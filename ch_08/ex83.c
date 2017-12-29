/* 
 * ex83.c Design and write _flushbuf , fflush and fclose .
 *
 * programm 8.2 and 8.3 from k&r; input.txt contains: abcdefghijk; don't include stdio.h
 * http://www.interpork.com
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define EOF        (-1)
#define BUFSIZ        7 // size of input buffer
#define OPEN_MAX    20  // max number of open files

typedef struct _iobuf   // struct for open file
{
    int cnt;            // number of remaining chars
    char *ptr;          // next char
    char *base;         // buffer's address
    struct              // flags
    {
        unsigned int is_read : 1;   // file open for reading
        unsigned int is_write : 1;  // file open for writing
        unsigned int is_unbuf : 1;  // file is unbuffered
        unsigned int is_eof : 1;    // we have end of file
        unsigned int is_err : 1;    // we have an error
    } flags;
    int fd;             // file's descriptor
} FILE;

FILE _iob[OPEN_MAX];    // array of open files

#define stdin (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);

#define feof(p)        (((p)->flags.is_eof) == 1)   // end of file?
#define ferror(p)    (((p)->flags.is_err) == 1)     //  is error?
#define fileno(p)    ((p)->fd)                      // get file descriptor

#define getc(p)        (--(p)->cnt >= 0 \
            ? (unsigned char) *(p)->ptr++ : _fillbuf(p))    // get next char
#define putc(x,p)    (--(p)->cnt >= 0 \
            ? *(p)->ptr++=(x) : _flushbuf((x),p))           // put next char
#define getchar()    getc(stdin)                        // get next char from stdin
#define putchar(x)    putc((x), stdout)                 // put next char into stdout

#define PERMS 0666          // RW for all

FILE *fopen(char *name, char *mode)     // open file, return file pointer
{
    int fd;
    FILE *fp;
    extern FILE _iob[];

    if(*mode != 'r' && *mode != 'w' && *mode != 'a')    // check correct mode
        return NULL;
    for(fp=_iob+2; fp < _iob+OPEN_MAX; fp++)        // find next free position in _iob[]
        if(fp->flags.is_read==0 && fp->flags.is_write==0)
            break;                  // we find free postition
    if(fp>=_iob+OPEN_MAX)           // no free position
        return NULL;
    
    if(*mode=='w')
        fd=creat(name, PERMS);
    else if(*mode=='a')
    {
        if((fd=open(name, O_WRONLY, 0))==-1)
            fd=creat(name, PERMS);
        lseek(fd, 0L, 2);
    }
    else
        fd=open(name, O_RDONLY, 0);
    if(fd==-1)              // it is impossible get access by name "name"
        return NULL;
    fp->fd=fd;
    fp->cnt=0;
    fp->base=NULL;
    (*mode=='r') ? (fp->flags.is_read=1) : (fp->flags.is_write=1);

    return fp;
}    

int _fillbuf(FILE *fp)      // ask system for memory and fill buffer
{
    int bufsize;
    
    if(fp->flags.is_read==0 || fp->flags.is_eof==1 || fp->flags.is_err==1)
        return EOF;
    bufsize=(fp->flags.is_unbuf==1) ? 1 : BUFSIZ;
    if(fp->base==NULL)      // no buffer so far
        if((fp->base=(char *) malloc(bufsize))==NULL)
            return EOF;     // system cannot give memory
    fp->ptr=fp->base;
    fp->cnt=read(fp->fd, fp->ptr, bufsize);     // fill the buffer
    if(--fp->cnt<0)
    {
        if(fp->cnt==-1)
            fp->flags.is_eof=1;
        else
            fp->flags.is_err=1;
        fp->cnt=0;
        return EOF;
    }
    
    return (unsigned char) *fp->ptr++;
}    

int _flushbuf(int c, FILE *fp)  // write data on disk
{
    int n_written=0, bufsize=0;

    if(fp->flags.is_write==0 || fp->flags.is_err==1)    // flags don't allow write data
        return EOF;
    bufsize=(fp->flags.is_unbuf==1) ? 1 : BUFSIZ;       // get size of buffer
    if(fp->base==NULL)      // if no buffer - create one
    {
        if((fp->base=(char *) malloc(bufsize))==NULL)
            return EOF;
    }
    else
    {
        n_written=write(fp->fd, fp->base, bufsize);     // write buffer's data on disk
        if(n_written!=bufsize)
        {
            fp->flags.is_err=1;
            return EOF;
        }
    }
    fp->ptr=fp->base;
    fp->cnt=bufsize;
    *fp->ptr++=c;
    fp->cnt--;

    return c;
}

int fflush(FILE *fp)    // flush given file or all open files(except std*)
{
    int n_written=0, bufsize=0;
    extern FILE _iob[];

    if(fp!=NULL)    // flush only given file
    {
        if(fp->flags.is_write==0 || fp->flags.is_err==1)    // flags don't allow write data
            return EOF;
        bufsize=(fp->flags.is_unbuf==1) ? 1 : BUFSIZ;       // get size of buffer
        n_written=write(fp->fd, fp->base, bufsize-fp->cnt); // write buffer's data on disk
        if(n_written!=bufsize-fp->cnt)
        {
            fp->flags.is_err=1;
            return EOF;
        }
        fp->ptr=fp->base;
        fp->cnt=bufsize;
    }
    else            // flush all open files (except stdin, stdout, stderr)
    {
        for(fp=_iob+3; fp<_iob+OPEN_MAX; fp++)
        {
            if(fp->flags.is_write==0)
                continue;
            if(fp->flags.is_err==1)     // flag don't allow write data
                return EOF;
            bufsize=(fp->flags.is_unbuf==1) ? 1 : BUFSIZ;   // get size of buffer
            n_written=write(fp->fd, fp->base, bufsize-fp->cnt); // write buffer's data on disk
            if(n_written!=bufsize-fp->cnt)
            {
                fp->flags.is_err=1;
                return EOF;
            }
            fp->ptr=fp->base;
            fp->cnt=bufsize;
        }
    }
    
    return 0;
}

int fclose(FILE *fp)    // close open file
{
    int bufsize, i;

    bufsize=(fp->flags.is_unbuf==1) ? 1 : BUFSIZ;
    if(fp->flags.is_write==1)
    {
        if(fflush(fp)==EOF)     // flush data on disk if file open for writing
            return EOF;
    }
    if(fp->base!=NULL)
    {
        fp->ptr=fp->base;
        for(i=0; i<bufsize; i++)    // fill buffer with zero
        {
            *fp->ptr++=0;
        }
    }
    free(fp->base);         // free allocated memory
    fp->flags.is_read=0;    // zeroing flags and properties
    fp->flags.is_write=0;
    fp->flags.is_unbuf=0;
    fp->flags.is_eof=0;
    fp->flags.is_err=0;
    fp->cnt=0;
    fp->ptr=NULL;
    fp->base=NULL;

    return 0;
}

int fseek(FILE *fp, long offset, int origin)    // set position
{
    long int pos;

    pos=lseek(fp->fd, 0L, SEEK_CUR);    // get current position

    if(fp->flags.is_write==1)   // flush data if any
        if(fflush(fp)<0)
            return -1;
    if((fp->flags.is_read==1) && (origin==SEEK_CUR))    // if file open for reading
    {
        if((pos=lseek(fp->fd, -(fp->cnt), SEEK_CUR))>=0)    // rewind buffer    
            if((pos=lseek(fp->fd, offset, origin))>=0)      // set position
            {
                fp->cnt=0;      // empty buffer
                return 0;
            }
        return -1;
    }

    // other cases: write, read/seek_set, read/seek_end
    if((pos=lseek(fp->fd, offset, origin))>=0)
    {
        if((fp->flags.is_read)==1)
        {
            fp->cnt=0;          // empty buffer
        }
        return 0;
    }
    else
        return -1;
}

int main(int argc, char *argv[])
{
    extern FILE _iob[];
    FILE *fpi, *fpo1, *fpo2, *fpo3, *fptemp;
    int c='w';
    int i;

    // create stdin
    _iob[0].cnt=0;
    _iob[0].ptr=(char *) 0;
    _iob[0].base=(char *) 0;
    _iob[0].flags.is_read=1;
    _iob[0].fd=0;    

    // create stdout
    _iob[1].cnt=0;
    _iob[1].ptr=(char *) 0;
    _iob[1].base=(char *) 0;
    _iob[1].flags.is_write=1;
    _iob[1].fd=1;    
  
    // create stderr  
    _iob[2].cnt=0;
    _iob[2].ptr=(char *) 0;
    _iob[2].base=(char *) 0;
    _iob[2].flags.is_write=1;
    _iob[2].flags.is_unbuf=1;
    _iob[2].fd=2;    

    fpi=fopen("input.txt", "r");
    fpo1=fopen("output.txt", "w");
    fpo2=fopen("output2.txt", "w"); // file will show all open files in _iob[]
    fpo3=fopen("output3.txt", "w");

    // show all open files (see output2.txt)
    for(fptemp=_iob; fptemp<_iob+OPEN_MAX; fptemp++)
    {
        if(fptemp->flags.is_write==0 && fptemp->flags.is_read==0)
            putc('0', fpo2);
        else
            putc('1', fpo2);
    }

    // copy 3 char from input.txt to output.txt
    for(i=0; i<3; i++)
    {
        c=getc(fpi);
        if(c==EOF)
            break;
        putc(c, fpo1);
    }

    // change position(+2) in input.txt and copy once more 3 chars
    fseek(fpi, 2, SEEK_CUR);
    for(i=0; i<3; i++)
    {
        c=getc(fpi);
        if(c==EOF)
            break;
        putc(c, fpo1);
    }

    // close 2 files and see again all open files (in output3.txt this time)
    fclose(fpo1);
    fclose(fpo2);
    for(fptemp=_iob; fptemp<_iob+OPEN_MAX; fptemp++)
    {
        if(fptemp->flags.is_write==0 && fptemp->flags.is_read==0)
            putc('0', fpo3);
        else
            putc('1', fpo3);
    }

    fclose(fpo3);

    return 0;
}

