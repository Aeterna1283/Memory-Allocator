/*alloc.c*/
#include "alloc.h"

extern heap *memspace;

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

    int8 *p;

    p = alloc(7);
    printf("%p\n", p);

    return 0;
}