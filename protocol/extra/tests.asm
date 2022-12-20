use16
global test_this_is
this_is:
    .a db 0x0
    .b db 0x0

use16
test_this_is:
    mov ah, 0x0E
    mov al, 'D'
    int 0x10

    push ebp
    mov ebp, esp

    mov si, [ebp + 8]

    pop ebp

    mov [this_is], si

    mov ah, [this_is.a]
    cmp ah, 0xAB
    je .u
    jne .m
.u:
    mov ah, 0x0E
    mov al, 'S'
    int 0x10

    ret
.m:
    mov ah, 0x0E
    mov al, 'E'
    int 0x10

    hlt
    ret

global do_something
do_something:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]

    pop ebp

    cmp eax, 0x10
    jne .f
    je .s
.f:
    mov ah, 0x0E
    mov al, 'F'
    int 0x10
    ret
.s:
    mov ah, 0x0E
    mov al, 'S'
    int 0x10
    ret