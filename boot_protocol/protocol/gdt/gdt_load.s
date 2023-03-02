;
;   __test_address: back-end stub
;
;       Test to see if a offset from a address surpassses a specific address
;
;       Input: 
;           uint16 ptr starting address(ax, ebp+8)
;           uint16 offset(bx, ebp+12)
;           uint16 conditional address(cx, ebp+16)
;       Output:
;           AH=`status`(1 if starting address + offset > conditional, 0 if starting address + offset < conditional)
;
global __load_gdt
use16
__load_gdt:
    ;mov eax, [g_GDT32_16_desc_addr]
    ;mov [g_GDTDesc], eax
    ;mov eax, [g_GDT32_16_address]
    ;mov [g_GDT32_16], eax
    
    in al, 0x92
	or al, 0x02
	out 0x92, al

	cli
	lgdt [g_GDT_desc_addr]
	mov eax, cr0
	or eax, 0x01
	mov cr0, eax

    jmp word 0x08:__init_pm