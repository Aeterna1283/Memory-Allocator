/*alloc.c*/
#include "alloc.h"

extern heap *memspace;

header *findblock_(header *hdr, word allocation, word n)
{
    bool okay;
    void *mem;
    header *hdr_;
    word n_;

    if((n + allocation) > (Maxwords - 2 ))
    {
        reterr(ErrNoMem);
    }


    okay = (!(hdr->w)) ? true :
    (!(hdr->allocated) && (hdr->w >= allocation)) ? true :
    false;


    if(okay)
    {
        return hdr;
    }

    else
    {
        mem = $v hdr + (hdr->w * 4) + 4;
        hdr_ = $h mem;
        n_ = n + hdr->w;

        return findblock_(hdr_, allocation, n_); //beautiful recursion

    }

    reterr(ErrUnknown);
}

void *mkalloc(word words, header *hdr)
{
    void *ret, *bytesin;
    word wordsin;

    //calculate where the header is with pointer arithmetic
    bytesin = ($v (($v hdr) - memspace));
    wordsin = ((uintptr_t)bytesin / 4) + 1;

    if(words > (Maxwords - wordsin))
    {
        reterr(ErrNoMem);
    }


    hdr->w = words;
    hdr->allocated = true;

    //calculate pointer address of the memspace
    ret = ($v hdr) + 4;

    return ret;
}

void *alloc(int32 bytes)
{
    //convert bytes to number of words
    word words;
    header *hdr;
    void *mem;
    //int32 ret;

    words = (!(bytes % 4)) ? bytes / 4 : (bytes/4) + 1;

    hdr = findblock(words); 
    if(!hdr)
    {
        return $v 0;
    }

    //hdr->w = 1;
    // printf("empty \n");
    // exit(0);

    if(words > Maxwords)
    {
        reterr(ErrNoMem);
    }

    mem = mkalloc(words, hdr);

    if(!mem)
    {
        return $v 0;
    }

    return mem;
}


int main(int argc, char *argv[])
{
    header *hdr;

    int8 *p;
    int8 *p2;
    int8 *p3;

    p = alloc(7);
    printf("Allocated1: %p\n", p);
    
    hdr = findblock(500);

    if(!hdr)
    {
        printf("Error %d\n", errno);
        return - 1;
    }

    printf("Memspace = %p\n", memspace);
    printf("Block = %p\n", hdr);

    p2 = alloc(2000);
    printf("Allocated2: %p\n", p2);

    p3 = alloc(1);
    printf("Allocated3: %p\n", p3);

    return 0;
}