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
init_pm:

    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov al, 'S'

    mov byte [0xB8000], al
    mov byte [0xB8002], 'E'
    mov byte [0xB8004], 'X'

    jmp 0x8:0x8500

%include "boot/gdt.asm"