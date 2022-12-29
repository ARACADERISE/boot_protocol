global save_gdt_and_load
use16
save_gdt_and_load:
    ; Do we need to load a already-working gdt into memory?
    mov eax, [g_GDT_status]
    cmp eax, 0
    je .load_new_gdt

    ; Is there already a bit32/bit16 GDT in memory that the user put there?
    cmp eax, 1
    je .do_it

    ; Is there already a bit32 GDT in memory?
    cmp eax, 2
    je .do_it

    ; If for some reason the gdt status is neither 1 or 0, error
    jmp .gdt_error
.load_new_gdt:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]
    mov [g_GDT_desc_addr], eax

    mov eax, [ebp + 12]
    mov [g_GDT_address], eax

    pop ebp

    mov eax, [g_GDT_desc_addr]
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

    jmp load_gdt

.failed:
    mov si, sum2
    call print
.hl:
    jmp .hl

sum db "Error loading the GDT :(", 0x0
sum2 db "Error loading sectors for kernel :(", 0x0