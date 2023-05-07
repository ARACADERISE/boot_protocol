use16
global enter_rmode

%include "protocol/gdt/enter_rmode.s"
%include "protocol/gdt/gdt_load.s"
%include "protocol/gdt/save_and_load_gdt.s"

;
; =======================
;       32-bit code
; =======================
;

;
;   __init_pm: back-end stub
;
;       Initialize 32-bit mode/assign `0x10` to segment registers
;
;       Input: 
;           None
;       Output:
;           None
;       On Error: This stub does not error
;
use32
__init_pm:

    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    jmp 0x8:0x9000

%include "boot/gdt.s"
    