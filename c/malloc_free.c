#include <unistd.h>

#define NALLOC 1024 /* min units */

typedef long Align;

union header {
    struct {
        union header *ptr;
        unsigned size;
    } s;
    Align x;
};

typedef union header Header;

static Header base;
static Header *freep = NULL;

void *my_malloc(unsigned nbytes);
void my_free(void *ap);

static Header *morecore(unsigned nunits)
{
    void *cp;
    Header *up;

    if (nunits < NALLOC)
        nunits = NALLOC;

    cp = sbrk(nunits * sizeof(Header));
    if (cp == (void*)(-1))
        return NULL;
    
    up = (Header *)cp;
    up->s.size = nunits;
    my_free((void*)(up + 1));
    return freep;
}

int main(void)
{
    int i;
    int n = 42;
    int *p = (int*)my_malloc(n * sizeof(int));
    
    for (i = 0; i < n; ++i) {
        p[i] = i;
    }

    /*
    for (i = -n; i < 2 * n; ++i) {
        p[i] = i;
    }
    */

    my_free(p);

    /*
    for (i = 0; i < n; ++i) {
        p[i] = i;
    }
    */

    return 0;
}

void *my_malloc(unsigned nbytes)
{
    Header *p, *prevp;
    unsigned nunits;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    if ((prevp = freep) == NULL) {
        base.s.ptr = prevp = freep = &base;
        base.s.size = 0;
    }
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
        if (p->s.size >= nunits) {
            if (p->s.size == nunits)
                prevp->s.ptr = p->s.ptr;
            else {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void*)(p + 1);
        }
        if (p == freep)
            if ((p = morecore(nunits)) == NULL)
                return NULL;
    }

    return NULL;
}

void my_free(void *ap)
{
    Header *bp, *p;

    bp = (Header*)ap - 1;
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
            break;

    if (bp + bp->s.size == p->s.ptr) {
        bp->s.size += p->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else
        bp->s.ptr = p->s.ptr;

    if (p + p->s.size == bp) {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else
        p->s.ptr = bp;
    
    freep = p; 
}
