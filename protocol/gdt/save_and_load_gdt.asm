global save_gdt_and_load
addr dw 0x0
use16
save_gdt_and_load:
    ; Do we need to load a already-working gdt into memory?
    mov eax, [g_GDT_status]
    cmp eax, 0
    je .load_new_gdt

    ; Is there already a GDT in memory that the user put there?
    cmp eax, 1
    je .do_it

    ; If for some reason the gdt status is neither 1 or 0, error
    jmp .gdt_error
.load_new_gdt:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]
    mov [g_GDT32_16_desc_addr], eax

    mov eax, [ebp + 12]
    mov [g_GDT32_16_address], eax

    pop ebp

    mov eax, [g_GDT32_16_desc_addr]
    mov [g_GDTDesc], eax

    mov ax, [g_GDTDesc.size]

    cmp ax, 0
    je .gdt_error

    mov ax, [g_GDTDesc.addr]
    cmp ax, 0
    je .gdt_error

    jmp .do_it
.gdt_error:
    mov si, sum
    call print

    jmp .hl
.do_it:
    ;push ebp
    ;mov ebp, esp

    ;mov ax, [ebp + 16]

    ;pop ebp

    ; Kernel cannot be loaded into memory at 0x0, 0x7C00 or 0x7E00
    ;cmp ax, 0x0
    ;je .failed
    ;cmp ax, 0x07C0
    ;je .failed
    ;cmp ax, 0x07E0
    ;je .failed

    ;mov es, ax
    ;xor bx, bx

    ;mov ah, 0x02
    ;mov al, 0x04
    ;mov ch, 0x00
    ;mov cl, 0x06
    ;mov dh, 0x00
    ;mov dl, 0x80
    ;int 0x13
    ;jc .failed

    jmp load_gdt

.failed:
    mov si, sum2
    call print
.hl:
    jmp .hl

sum db "Error loading the GDT :(", 0x0
sum2 db "Error loading sectors for kernel :(", 0x0