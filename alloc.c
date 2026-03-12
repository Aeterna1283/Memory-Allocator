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
        mem = $v hdr + hdr->w;
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

    mem = $v memspace;
    hdr = $h mem; 

    //hdr->w = 1;

    if(!(hdr->w)) 
    {
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

    else
     {
        // printf("blank\n");
        // exit(0);
        (void)0;
    }

    return $v 0;

}


int main(int argc, char *argv[])
{
    header *p;

    // int8 *p;

    // p = alloc(7);
    // printf("%p
    p = findblock(500);

    if(!p)
    {
        printf("Error %d\n", errno);
        return - 1;
    }

    printf("%p\n", memspace);
    printf("%p\n", p);

    return 0;
}