;
;   __read_disk: back-end stub
;
;       Read X amount of sectors into memory address Y
;
;       Input: 
;           uint16 sectors(ax, ebp + 8, assigned to `sector_count`)
;           uint16 address(ax, ebp + 12, assigned to `address`)
;       Output:
;           None
;       On Error: halt
;
global __read_disk
sector_count        dw 0x0
address             dw 0x0
__read_disk:
    push ebp
    mov ebp, esp

    mov ax, [ebp + 8]
    mov [sector_count], ax

    mov ax, [ebp + 12]
    mov [address], ax

    pop ebp

    mov ax, [address]
    mov es, ax
    xor bx, bx

    mov ah, 0x02
    mov al, [sector_count]
    mov ch, 0x00
    mov cl, 0x08
    mov dh, 0x00
    mov dl, 0x80
    int 0x13
    jc .__read_disk_failed

    jmp 0x0:0xA000

    ret
.__read_disk_failed:
    mov si, disk_failure
    call __asm_print

.__loop:
    jmp .__loop

disk_failure: db "Read disk error :(", 0xD, 0xA, 0x0