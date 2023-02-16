use16

global test_read
test_read:
    mov ax, 0x07E0
    mov es, ax
    xor bx, bx

    mov ah, 0x02
    mov al, 0x09
    mov ch, 0x00
    mov cl, 0x03
    mov dh, 0x00
    mov dl, 0x80
    int 0x13

    mov ax, 0x0A00
    mov es, ax
    xor bx, bx

    mov ah, 0x02
    mov al, 0x06
    mov ch, 0x00
    mov cl, 0x0C
    mov dh, 0x00
    mov dl, 0x80
    int 0x13

    jmp 0x0:0x7E00