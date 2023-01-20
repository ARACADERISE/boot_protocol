;
;   __save_gdt_and_load: back-end stub
;
;       Save the GDT/GDT description into memory(if need-be), then load the GDT description and jump
;       to 32-bit mode.
;
;       Input: 
;           _gdt_desc gdt_desc(eax, ebp + 8, assigned to `g_GDT_desc_addr`)
;           _GDT gdt(eax, ebp + 12, assigned to `g_GDT_address`)
;       Output:
;           None
;       On Error: halts
;
global __save_gdt_and_load
use16
__save_gdt_and_load:
    ; Do we need to load a already-working gdt into memory?
    mov eax, [g_GDT_status]
    cmp eax, 0
    je .load_new_gdt

    ; Is there already a bit32/bit16 GDT in memory that the user put there?
    cmp eax, 1
    je .load_it

    ; Is there already a bit32 GDT in memory?
    cmp eax, 2
    je .load_it

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

    jmp .load_it
.gdt_error:
    mov si, gdt_load_error
    call __asm_print

    jmp .hl
.load_it:

    jmp __load_gdt

.failed:
    mov si, sector_read_error
    call __asm_print
.hl:
    jmp .hl

gdt_load_error db "Error loading the GDT :(", 0x0
sector_read_error db "Error loading sectors for kernel :(", 0x0