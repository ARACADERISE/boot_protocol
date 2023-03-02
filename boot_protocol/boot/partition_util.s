use16

global load_program_for_higher_half_kernel
load_program_for_higher_half_kernel:
    push bp
    mov bp, sp

    ; Second stage ending address
    ; The value passed to this(ex 0x9000) will have to be divided by 16 to get the
    ; correct value to get the correct physical address using segment:offset
    ; 0x9000 / 16 = 0x900 * 16 = 0x9000 physical address.
    mov ax, [bp + 8]

    ; Sector number that we are on
    mov cl, [bp + 16]

    pop bp

    ; For now, this is okay. We need to either locate it at an address we know won't get overwritten,
    ; or somehow take in arguments.
    mov ax, 0x0900
    mov es, ax
    xor bx, bx

    ; This program is only 1 sector in size
    mov ah, 0x02
    mov al, 0x01
    mov ch, 0x00
    mov cl, 0x0D
    mov dh, 0x00
    mov dl, 0x80
    int 0x13
    jmp 0x0:0x9000

    ret

global read_in_program
read_in_program:

    ; Address of program
    pop ax
    mov es, ax
    xor bx, bx

    ; Sector amount
    mov ah, 0x02
    pop ax

    ; Sector starting number
    mov ch, 0x0
    pop cx

    mov dh, 0x00
    mov dl, 0x80
    int 0x13
    jc .f

    mov ah, 0x0E
    mov al, 'H'
    int 0x10
    ret
.f:
    mov ah, 0x0E
    mov al, 'F'
    int 0x10
    cli
    hlt

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
    mov al, 0x07
    mov ch, 0x00
    mov cl, 0x0D
    mov dh, 0x00
    mov dl, 0x80
    int 0x13

    jmp 0x0:0x7E00