use16
global enter_rmode

%%include "protocol/gdt/enter_rmode.s"
%%include "protocol/gdt/gdt_load.s"
%%include "protocol/gdt/save_and_load_gdt.s"

;
; =======================
;       32-bit code
; =======================
;
use32
init_pm:

    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    jmp 0x8:0x{0}

%%include "boot/gdt.s"
    