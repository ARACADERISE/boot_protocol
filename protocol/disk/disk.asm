global read_disk
sector_count        dw 0x0
address             dw 0x0
read_disk:
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
    jc .read_disk_failed

    jmp 0x0:0xA000

    ret
.read_disk_failed:
    mov si, disk_failure
    call print

.loop:
    jmp .loop

disk_failure: db "Read disk error :(", 0xD, 0xA, 0x0