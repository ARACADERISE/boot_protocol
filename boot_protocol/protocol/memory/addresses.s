; Address for storing the GDT struct
g_GDT_address       equ 0x0900

; Address for storing GDT description struct
; g_GDT_address can take up to 40 bytes. We set the address of g_GDT_desc_addr to 40 bytes + 2 for padding
g_GDT_desc_addr     equ 0x092A

; Address for storing status on GDT
; Status:
;   0 - A GDT/GDT description needs to be loaded into memory via `save_gdt_and_load`
;   1 - A 16-bit/32-bit GDT/GDT description has been loaded into memory
;   2 - A 32-bit only GDT/GDT description has been loaded into memory
g_GDT_status:       equ 0x0938