org 0x7C00
use16

xor ax, ax
mov es, ax
mov ds, ax

cli
mov bp, 0x7C00
mov sp, bp
mov ss, ax
sti

mov ax, [Info.second_stage_addr]
mov es, ax
xor bx, bx

mov ah, 0x02
mov al, [Info.second_stage_size]
mov ch, 0x00
mov cl, [Info.sector]
mov dh, 0x00
mov dl, 0x80
int 0x13
jc failed

mov ax, [Info.second_stage_size]
add [Info.sector], ax
mov [Info.sector], ax

mov ax, [Info.kernel_addr]
mov es, ax
xor bx, bx

mov ah, 0x02
mov al, [Info.kernel_size]
mov ch, 0x00
mov cl, [Info.sector]
mov dh, 0x00
mov dl, 0x80
int 0x13
jc failed

mov ax, [Info.kernel_size]
add [Info.sector], ax
mov ax, [Info.sector]
mov [sector_addr], ax

jmp word 0x0:0x7e00

jmp $

sector_addr		equ 0x5000

Info:
	; Sector information
	.sector				db 0x02
	; Second stage information
	.second_stage_size	dw ((second_stageE - second_stage) / 512) + 2
	.second_stage_addr	dw 0x07e0
	.second_stage_loc	dw 0x7e00
	; Kernel information
	.kernel_size		dw ((kernelE - kernel) / 512) + 1
	.kernel_addr		dw 0x0900
	.kernel_loc			dw 0x9000

failed:
.failed_loop:
	jmp .failed_loop

times 510 - ($ - $$) db 0x0
dw 0xAA55

second_stage: incbin "bin/boot.bin"
second_stageE:

kernel: incbin "bin/kernel.bin"
kernelE:
    