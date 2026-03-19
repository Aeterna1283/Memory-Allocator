/*alloc.c*/
#include "alloc.h"

extern heap *memspace; //links with global memspace from heap.asm after compilation when both are .o files

public void zero(int8 *str, int16 size)
{
    int8 *p;
    int16 n;

    for (n=0, p=str; n<size; n++, p++)
    {
        *p = 0;
    }
    

    return;
}

public bool destroy(void *addr)
{
    header *p;
    int16 n;
    //cannot calc first word of header because the adress is the first word of the allocation
    //which means going back one word reaches the header

    p = ($h addr) - 4;
    if(!(p->w) || (!(p->allocated)))
        reterr(Err2xFree );
    
    n = (p->w * 4);
    zero($1 addr, n);
    p->allocated = false;

    return true;
}

private header *findblock_(header *hdr, word allocation, word n)
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

private void *mkalloc(word words, header *hdr)
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

public void *alloc(int32 bytes)
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

private void show_(header *hdr)
{
    header *p;
    void *mem;
    int32 n;

    for(n = 1, p = hdr; p->w; mem=$v p + ((p->w + 1) * 4), p=mem, n++)
    {
        printf("Alloc %d = %d %s words\n", n, p->w, (p->allocated) ? "allocated" : "free");

    }
    return;
}


int main(int argc, char *argv[])
{
    header *hdr;

    int8 *p;
    int8 *p2;
    int8 *p3;
    int8 *p4;
    bool end;

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
    printf("Allocated3: %p\n\n", p3);

    p4 = alloc(1800);

    end = destroy(p2);
    printf("\n end = %s\n", (end)?"true" : "false");

    show();

    return 0;
}