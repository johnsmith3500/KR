/*

 * exercise 8.5 from k&r; tested on freebsd 10.3

 * http://www.interpork.com

 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#define KRNAME_MAX 255
#define KRMAX_PATH 1024
/* dirctory entity */
typedef struct
{
    long ino;
    char name[KRNAME_MAX+1];
} KRdirent;

typedef struct
{
    int fd;
    KRdirent d;
} KRDIR;

KRDIR *kropendir(char *dirname);
KRdirent *krreaddir(KRDIR *dfd);
void krclosedir(KRDIR *dfd);
void krdirwalk(char *dir, void (*fcn)(char *));
void krfsize(char *name);
int main(int argc, char *argv[])
{
    if(argc==1)
        krfsize(".");
    else
        while(--argc>0)
            krfsize(*++argv);        

    return 0;
}
/* krfsize() print info about given file or directory (size, etc) */
void krfsize(char *name)
{
    struct stat stbuf;
    int mode;
    char *str, *tstr;
    
    if(stat(name, &stbuf)==-1)
    {
        fprintf(stderr, "fsize: cannot access %s\n", name);
        perror("fsize: ");
        return;
    }
    if((stbuf.st_mode & S_IFMT)==S_IFDIR)
    {
        krdirwalk(name, krfsize);
    }

/*    translating bits in human readable format     */
    mode=stbuf.st_mode & S_IFMT;
    switch(mode)
    {
        case 0010000:
            str="S_IFIFO: named pipe (fifo)";
            break;
        case 0020000:
            str="S_IFCHR: character special";
            break;
        case 0040000:
            str="S_IFDIR: directory";
            break;
        case 0060000:
            str="S_IFBLK: block special";
            break;
        case 0100000:
            str="S_IFREG: regular";
            break;
        case 0120000:
            str="S_IFLNK: symbolic link";
            break;
        case 0140000:
            str="S_IFSOCK: socket";
            break;
        default:
            str="unknown file type";
            break;
    }

    printf("%s\n", name);
    printf("\t%-12s%10d\n", "st_dev", stbuf.st_dev);
    printf("\t%-12s%10d\n", "st_ino", stbuf.st_ino);
    printf("\t%-12s%10o octal\t%s\n", "st_mode", stbuf.st_mode, str);
    printf("\t%-12s%10d\n", "st_nlinks", stbuf.st_nlink);
    printf("\t%-12s%10d\n", "st_uid", stbuf.st_uid);
    printf("\t%-12s%10d\n", "st_gid", stbuf.st_gid);
    printf("\t%-12s%10d\n", "st_rdev", stbuf.st_rdev);
    tstr=ctime(&stbuf.st_atim.tv_sec);
    printf("\t%-12s%10s", "st_atim", tstr);
    tstr=ctime(&stbuf.st_mtim.tv_sec);
    printf("\t%-12s%10s", "st_mtim", tstr);
    tstr=ctime(&stbuf.st_ctim.tv_sec);
    printf("\t%-12s%10s", "st_ctim", tstr);
    printf("\t%-12s%10ld\n", "st_size", stbuf.st_size);
    printf("\t%-12s%10ld\n", "st_blocks", stbuf.st_blocks);
    printf("\t%-12s%10d\n", "st_blksize", stbuf.st_blksize);
    printf("\t%-12s%10d\n", "st_flags", stbuf.st_flags);
    printf("\t%-12s%10d\n", "st_gen", stbuf.st_gen);
    tstr=ctime(&stbuf.st_birthtim.tv_sec);
    printf("\t%-12s%10s", "st_birthtim", tstr);
    printf("\n");
/*    printf("%8ld %s\n", stbuf.st_size, name);        */
}
void krdirwalk(char *dir, void (*fcn)(char *))
{
    char name[KRMAX_PATH];
    KRdirent *dp;
    KRDIR *dfd;

    if((dfd=kropendir(dir))==NULL)
    {
        fprintf(stderr, "krdirwalk: cannot open %s\n", dir);
        return;
    }
    while((dp=krreaddir(dfd))!=NULL)
    {
        if(strcmp(dp->name, ".")==0 || strcmp(dp->name, "..")==0)
            continue;
        if(strlen(dir)+strlen(dp->name)+2 > sizeof name)
            fprintf(stderr, "krdirwalk: %s/%s too long\n", dir, dp->name);
        else
        {
            sprintf(name, "%s/%s", dir, dp->name);
            (*fcn)(name);
        }
    }
    krclosedir(dfd);
}

/* read()ing directory as a file */
KRdirent *krreaddir(KRDIR *dp)
{
    struct dirent dirbuf;  /* directory struct on the given system */

                                        /* in freebsd it is struct dirent{} in dirent.h */

                                        /* IMPORTANT! : size of the struct can vary */           
    static KRdirent d;
    int n=0;
    long int oldpos, pos;
/*    save current position in file in oldpos var        */
    oldpos=lseek(dp->fd, 0, SEEK_CUR);
    while((n=read(dp->fd, (char *)&dirbuf, sizeof dirbuf))>0)
    {
        pos=lseek(dp->fd, oldpos+dirbuf.d_reclen, SEEK_SET);
        if(dirbuf.d_fileno==0)
        {
            oldpos+=dirbuf.d_reclen;
            continue;    
        }
        d.ino=dirbuf.d_fileno;
        strncpy(d.name, dirbuf.d_name, KRNAME_MAX);
        d.name[KRNAME_MAX]='\0';
        return &d;
    }
    return NULL;
}


/* open directory (as a file) for read()ing */
KRDIR *kropendir(char *dirname)
{
    int fd;
    struct stat stbuf;
    KRDIR *dp;

    if((fd=open(dirname, O_RDONLY, 0))==-1
            || fstat(fd, &stbuf)==-1
            || (stbuf.st_mode & S_IFMT)!=S_IFDIR
            || (dp=(KRDIR*)malloc(sizeof(KRDIR)))==NULL)
        return NULL;
    dp->fd=fd;
    return dp;
}



/* close directory opened by kropendir */
void krclosedir(KRDIR *dp)
{
    if(dp)
    {
        close(dp->fd);
        free(dp);
    }
}