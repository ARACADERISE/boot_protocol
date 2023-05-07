use16

;
; =======================
;       16-bit code
; =======================
;

%include "protocol/memory/addresses.s"
%include "protocol/memory/address_ideals.s"
%include "protocol/extra/tests.s"
%include "protocol/mouse/mouse.s"
%include "protocol/tools/print.s"
%include "protocol/disk/disk.s"

; TODO: Make these into "magic numbers"
; For now, this works well
; TODO: For now, these are no longer needed. Might add them back later tho
;%define clean_gdt           0x1
;%define bit32_bit16_gdt     0x10
;%define bit32_only_gdt      0x2
;%define b8000_vid_mode      0x20
;%define vesa_vid_mode       0x3

%define DEFAULT_ALL                 clean_gdt | b8000_vid_mode
%define CLEAN_GDT_DEF_VID_MODE      clean_gdt | b8000_vid_mode
%define CLEAN_GDT_VESA_VID_MODE     clean_gdt | vesa_vid_mode
%define BIT32_BIT16_DEF_VID_MODE    bit32_bit16_gdt | b8000_vid_mode
%define BIT32_BIT16_VESA_VID_MODE   bit32_bit16_gdt | vesa_vid_mode
%define BIT32_ONLY_DEF_VID_MODE     bit32_only_gdt | b8000_vid_mode
%define BIT32_ONLY_VESA_VID_MODE    bit32_only_gdt | vesa_vid_mode

setting         db 0x0

;
;   init_bootloader: "front-end" stub
;
;       Load a already-working or clean GDT/GDT description into memory. Check memory map.
;       Load user into B8000 mode or VESA video mode.
;
;       Input: 
;           int8 setting(ah, ebp + 8)
;       Output:
;           None
;       On Error: halts
;
global init_bootloader
use16
init_bootloader:
    mov ah, 0x00
    mov al, 0x03
    int 0x10

    ; Init the mouse
    call __config_mouse

.defall:
    mov eax, [g_GDTDesc]
    mov [g_GDT_desc_addr], eax

    mov eax, [g_GDT32_16]
    mov [g_GDT_address], eax

    mov eax, 0                        ; 0 meaning: there isn't an already-working GDT loaded into  memory
    mov [g_GDT_status], eax

    ret
.error:
    mov si, invalid
    call __asm_print
.error_hlt:
    jmp .error_hlt
    
invalid: db "Invalid setting passed to `init_bootloader`", 0x0D, 0x0A, 0x00
%include "protocol/gdt/gdt_ideals.s"