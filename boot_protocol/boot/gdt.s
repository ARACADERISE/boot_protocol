working_bit32_bit16_GDT:      ; NULL descriptor
            .null_desc          dq 0x0

            ; 32-bit code segment
            .code32_limit       dw 0xFFFF                   ; limit (bits 0-15) = 0xFFFFF for full 32-bit range
            .code32_base        dw 0                        ; base (bits 0-15) = 0x0
            .code32_base2       db 0                        ; base (bits 16-23)
            .code32_acces       db 10011010b                ; access (present, ring 0, code segment, executable, direction 0, readable)
            .code32_gran        db 11001111b                ; granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
            .code32_base_high   db 0                        ; base high

            ; 32-bit data segment
            .data32_limit       dw 0FFFFh                   ; limit (bits 0-15) = 0xFFFFF for full 32-bit range
            .data32_base        dw 0                        ; base (bits 0-15) = 0x0
            .data32_base2       db 0                        ; base (bits 16-23)
            .data32_access      db 10010010b                ; access (present, ring 0, data segment, executable, direction 0, writable)
            .data32_gran        db 11001111b                ; granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
            .data32_base_high   db 0                       ; base high

            ; 16-bit code segment
            .cod16_limit        dw 0FFFFh                   ; limit (bits 0-15) = 0xFFFFF
            .code16_base        dw 0                        ; base (bits 0-15) = 0x0
            .code16_base2       db 0                        ; base (bits 16-23)
            .code16_access      db 10011010b                ; access (present, ring 0, code segment, executable, direction 0, readable)
            .code16_gran        db 00001111b                ; granularity (1b pages, 16-bit pmode) + limit (bits 16-19)
            .code16_base_high   db 0                        ; base high

            ; 16-bit data segment
            .data16_limit       dw 0FFFFh                   ; limit (bits 0-15) = 0xFFFFF
            .data16_base        dw 0                        ; base (bits 0-15) = 0x0
            .data16_base2       db 0                        ; base (bits 16-23)
            .data16_access      db 10010010b                ; access (present, ring 0, data segment, executable, direction 0, writable)
            .data16_gran        db 00001111b                ; granularity (1b pages, 16-bit pmode) + limit (bits 16-19)
            .data16_base_high   db 0                        ; base high

working_bit32_bit16_GDTDesc:
    dw working_bit32_bit16_GDTDesc - working_bit32_bit16_GDT - 1
    dd working_bit32_bit16_GDT

working_bit32_GDT:
            .null_desc1          dq 0x0

            ; 32-bit code segment
            .code32_limit1       dw 0xFFFF                   ; limit (bits 0-15) = 0xFFFFF for full 32-bit range
            .code32_base1        dw 0                        ; base (bits 0-15) = 0x0
            .code32_base21       db 0                        ; base (bits 16-23)
            .code32_acces1       db 10011010b                ; access (present, ring 0, code segment, executable, direction 0, readable)
            .code32_gran1        db 11001111b                ; granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
            .code32_base_high1   db 0                        ; base high

            ; 32-bit data segment
            .data32_limit1       dw 0FFFFh                   ; limit (bits 0-15) = 0xFFFFF for full 32-bit range
            .data32_base1        dw 0                        ; base (bits 0-15) = 0x0
            .data32_base21       db 0                        ; base (bits 16-23)
            .data32_access1      db 10010010b                ; access (present, ring 0, data segment, executable, direction 0, writable)
            .data32_gran1        db 11001111b                ; granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
            .data32_base_high1   db 0                       ; base high

working_bit32_GDTDesc:
    dw working_bit32_GDTDesc - working_bit32_GDT - 1
    dd working_bit32_GDT

g_GDT32_16:      
            ; NULL descriptor
            .n                      dq 1

            ; 32-bit code segment
            .ccode32_limit          dw 0                         ; limit (bits 0-15) = 0xFFFFF for full 32-bit range
            .ccode32_base           dw 0                        ; base (bits 0-15) = 0x0
            .ccode32_base2          db 0                        ; base (bits 16-23)
            .ccode32_access         db 0                        ; access (present, ring 0, code segment, executable, direction 0, readable)
            .ccode32_gran           db 0                        ; granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
            .ccode32_base_high      db 0                        ; base high

            ; 32-bit data segment
            .ddata32_limit          dw 0                        ; limit (bits 0-15) = 0xFFFFF for full 32-bit range
            .ddata32_base           dw 0                        ; base (bits 0-15) = 0x0
            .ddata32_base2          db 0                        ; base (bits 16-23)
            .ddata32_access         db 0                        ; access (present, ring 0, data segment, executable, direction 0, writable)
            .ddata32_gran           db 0                        ; granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
            .ddata32_base_high      db 0                        ; base high

            ; 16-bit code segment
            .ccode16_limit                  dw 0                        ; limit (bits 0-15) = 0xFFFFF
            .ccode16_base                   dw 0                        ; base (bits 0-15) = 0x0
            .ccode16_base2                  db 0                        ; base (bits 16-23)
            .ccode16_access                 db 0                        ; access (present, ring 0, code segment, executable, direction 0, readable)
            .ccode16_gran                   db 0                        ; granularity (1b pages, 16-bit pmode) + limit (bits 16-19)
            .ccode16_base_high              db 0                        ; base high

            ; 16-bit data segment
            .ddata16_limit                  dw 0                        ; limit (bits 0-15) = 0xFFFFF
            .ddata16_base                   dw 0                        ; base (bits 0-15) = 0x0
            .ddata16_base2                  db 0                        ; base (bits 16-23)
            .ddata16_access                 db 0                        ; access (present, ring 0, data segment, executable, direction 0, writable)
            .ddata16_gran                   db 0                ; granularity (1b pages, 16-bit pmode) + limit (bits 16-19)
            .ddata16_base_high              db 0                        ; base high

g_GDTDesc:
    .size dw 0x0001
    .addr dd 0x000000
