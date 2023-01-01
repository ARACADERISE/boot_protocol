use16
global enter_rmode

%include "protocol/gdt/enter_rmode.asm"
%include "protocol/gdt/gdt_load.asm"
%include "protocol/gdt/save_and_load_gdt.asm"

;
; =======================
;       32-bit code
; =======================
;
use32
address equ 0x500A
init_pm:

    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    jmp 0x8:address

%include "boot/gdt.asm"