global load_gdt

use16
load_gdt:
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

    jmp word 0x08:init_pm