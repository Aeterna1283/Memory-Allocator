bits 64 ;64 bit systems
global memspace
%define Heapsize (1024*1024*1024/8)  

Section .data alloc noexec write progbit

Section .heap alloc noexec write nobits
    memspace:
        heapsize equ Heapsize 
        resq heapsize ;quad word is reserved

Section .note.GNU-stack noalloc

; to compile: nasm -f elf64 heap.asm
; to link: ld heap.o -o temp
; run ./temp