/*

 * exercise 8.6 from paragraph 8.7 k&r
 * http://www.interpork.com
 */
#include <unistd.h>
#include <stdio.h>
#define NALLOC 1024    /* minimum memory units for request */
typedef long Align;    /* for alignment by long */
union header        /* header of memory block */
{
    struct
    {
        union header *ptr;    /* next memory block in list of free blocks */
        unsigned size;        /* size of this block */
    } s;
    Align x;            /* block alignment */
};
typedef union header Header;        
static Header base;        /* empty list for starting */
static Header *freep=NULL;    /* beginning in list of free blocks */
void *krmalloc(unsigned nbytes);    /* memory dispatcher */
static Header *krmorecore(unsigned nu);    /* ask memory from system */
void krfree(void *ap);        /* include a block in list of free blocks */
void *mycalloc(int n, long size); /* my version of calloc() */
struct cd    /* struct for mycalloc() demo */
{
    int i;
    struct cd *cdptr;
    char buf[16];
    long size;
};
int fillcd(struct cd *acd);    /* get some data from user for mycalloc() demo */
void printcd(struct cd *acd);    /* print out struct cd */

int main(int argc, char *argv[])
{
    struct cd *root;    /* initial address */
    int i;
    int ncd=3;        /* how many (struct cd) create */

    if((root=(struct cd *)mycalloc(ncd, sizeof (struct cd)))==NULL)
    {
        perror("mycalloc()");
        return 1;
    }
    for(i=0; i<ncd-1; i++)    /* leave one empty */
        fillcd(root+i);
    for(i=0; i<ncd; i++)
        printcd(root+i);
    krfree(root);
    
    return 0;
}

void *mycalloc(int n, long size)
{
    long total, i;
    void *vptr;

    total=n*size;
    vptr=krmalloc(total);
    if(vptr==NULL)
        return NULL; /* no more memory */
    for(i=0; i<total; i++)
        *((char *)vptr+i)=0;
    
    return vptr;
}

int fillcd(struct cd *acd)
{
    printf("Enter integer: ");
    scanf("%d", &acd->i);
    acd->cdptr=acd;
    printf("Enter a small string: ");
    scanf("%s", acd->buf);
    acd->size=sizeof *acd;
    return 1;
}
void printcd(struct cd *acd)
{
    printf("------------------\n");
    printf("%18s: %d\n", "int i", acd->i);
    printf("%18s: %p\n", "struct cd *cdptr", acd->cdptr);
    printf("%18s: %s\n", "char bur[16]", acd->buf);
    printf("%18s: %ld\n", "long size", acd->size);
    printf("------------------\n");
}

void krfree(void *ap)
{
    Header *bp, *p;

    bp=(Header *)ap-1;    /* pointer at header of block */
    for(p=freep; !(bp>p && bp<p->s.ptr); p=p->s.ptr)
        if(p>=p->s.ptr && (bp>p || bp<p->s.ptr))
            break;        /* free block in the begin or in the end */
    if(bp+bp->s.size==p->s.ptr)    /* combine with upper neighbor */
    {
        bp->s.size+=p->s.ptr->s.size;
        bp->s.ptr=p->s.ptr->s.ptr;
    }
    else
        bp->s.ptr=p->s.ptr;
    if(p+p->s.size==bp)        /* combine with lower neighbor */
    {
        p->s.size+=bp->s.size;
        p->s.ptr=bp->s.ptr;
    }
    else
        p->s.ptr=bp;
    freep=p;
}
    

static Header *krmorecore(unsigned nu)
{
    char *cp;
    Header *up;

    if(nu<NALLOC)
        nu=NALLOC;
    cp=sbrk(nu*sizeof(Header));
    if(cp==(char *)-1)    /* no more memory */
        return NULL;
    up=(Header *)cp;
    up->s.size=nu;
    krfree((void *)(up+1));
    
    return freep;
}

void *krmalloc(unsigned nbytes)
{
    Header *p, *prevp;
    unsigned nunits;

    nunits=(nbytes+sizeof(Header)-1)/sizeof(Header)+1;
    if((prevp=freep)==NULL)        /* there is no list of free memory yet */
    {
        base.s.ptr=freep=prevp=&base;
        base.s.size=0;
    }
    for(p=prevp->s.ptr; ;prevp=p, p=p->s.ptr)
    {
        if(p->s.size>=nunits)        /* block is big enough */
        {
            if(p->s.size==nunits)    /* exactly right size */
                prevp->s.ptr=p->s.ptr;
            else            /* cut tail part */
            {
                p->s.size-=nunits;
                p+=p->s.size;
                p->s.size=nunits;
            }
            freep=prevp;
            return (void *)(p+1);
        }
        if(p==freep)        /* have walk through whole list */
            if((p=krmorecore(nunits))==NULL)
                return NULL;    /* no more memory */
    }
}
            
