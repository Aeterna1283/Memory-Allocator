/*alloc.c*/
#include "alloc.h"

extern heap *memspace;

void *alloc(int32 bytes)
{
    //convert bytes to number of words
    word words;

    words = (!(bytes % 4)) ? bytes / 4 : (bytes/4) + 1;

    //find hole
    
}


int main(int argc, char *argv[])
{
    


    return 0;
}