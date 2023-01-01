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
kernel_address_at       equ 0x8F00
init_pm:

    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    jmp 0x8:kernel_address_at

%include "boot/gdt.asm"