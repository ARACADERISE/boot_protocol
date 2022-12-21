import yaml
import os
import sys

if os.path.isfile('../boot.yaml'):
    REQUIRED = [
        'kernel_binary',
        'kernel_addr',
        'kernel_source_code_file',
        'second_stage_binary',
        'second_stage_addr',
        'second_stage_source_code_file'
    ]

    file = open('../boot.yaml', 'r')
    data = yaml.safe_load(file)
    file.close()

    second_stage_source_code_file = data['second_stage_source_code_file']
    second_stage_source_code_file_o = second_stage_source_code_file.split('/')
    for i in range(len(second_stage_source_code_file_o)):
        if not '.' in second_stage_source_code_file_o[i]: del second_stage_source_code_file_o[i]
        if i == len(second_stage_source_code_file_o)-1: break
    
    second_stage_source_code_file_o = second_stage_source_code_file_o[0]
    second_stage_source_code_file_o = second_stage_source_code_file_o[:len(second_stage_source_code_file_o)-2]
    second_stage_source_code_file_o = f'{second_stage_source_code_file_o}.o'

    kernel_source_code_file = data['kernel_source_code_file']
    kernel_source_code_file_o = kernel_source_code_file.split('/')
    for i in range(len(kernel_source_code_file_o)):
        if not '.' in kernel_source_code_file[i]: del kernel_source_code_file_o[i]
        if i == len(kernel_source_code_file_o)-1: break
    
    kernel_source_code_file_o = kernel_source_code_file_o[0]
    kernel_source_code_file_o = kernel_source_code_file_o[:len(kernel_source_code_file_o)-2]
    kernel_source_code_file_o = f'{kernel_source_code_file_o}.o'

    for i in REQUIRED:
        if not i in data:
            print(f'Error: Expected `{i}` in `boot.yaml`.')
            sys.exit(1)

    file = open('../'+data['kernel_binary'], 'rb')
    kernel_binary_data = file.read();
    file.close()

    file = open('../'+data['second_stage_binary'], 'rb')
    second_stage_binary_data = file.read()
    file.close()

    # Making the hex value of ss_addr look good
    ss_addr = str(hex(data['second_stage_addr']))
    ss_addr = ss_addr[2:]
    ss_addr = f'0x0{ss_addr}'
    r_ss_addr = ss_addr[3:]
    r_ss_addr = f'0x{r_ss_addr}0'

    # Getting the size(in sectors)
    ss_size = int(len(second_stage_binary_data)/512)

    # Checking the size
    if ss_size * 512 < len(second_stage_binary_data):
        ss_size += 1
    
    # Making the hex value of ss_size look good
    ss_size = str(hex(ss_size))
    ss_size = ss_size[2:]
    ss_size = f'0x0{ss_size}'

    # Making the hex value of kern_addr look good
    kern_addr = str(hex(data['kernel_addr']))
    kern_addr = kern_addr[2:]
    kern_addr = f'0x0{kern_addr}'
    r_kern_addr = kern_addr[3:]
    r_kern_addr = f'0x{r_kern_addr}0'

    # Getting the size(in sectors)
    kern_size = int(len(kernel_binary_data)/512)

    # Checking the size
    if kern_size * 512 < len(kernel_binary_data):
        kern_size += 1
    
    # Making the hex value of kern_size look good
    kern_size = str(hex(kern_size))
    kern_size = kern_size[2:]
    kern_size = f'0x0{kern_size}'

    sector_for_kern = hex(0x02 + int(ss_size, base=16)-1)
    sector_for_kern = str(sector_for_kern)
    sector_for_kern = sector_for_kern[2:]
    sector_for_kern = f'0x0{sector_for_kern}'

    # The code :D
    boot = f"""org 0x7C00
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
mov cl, {sector_for_kern}
mov dh, 0x00
mov dl, 0x80
int 0x13
jc failed

mov ax, [Info.kernel_size]
add [Info.sector], ax

mov ax, [Info.sector]
mov [sector_addr], ax

jmp word 0x0:{r_ss_addr}

jmp $

sector_addr		equ 0x5000

Info:
	; Sector information
	.sector				db 0x02
	; Second stage information
	.second_stage_size	dw (second_stageE - second_stage) / 512 + 2
	.second_stage_addr	dw 0x07E0
	.second_stage_loc	dw 0x7E00
	; Kernel information
	.kernel_size		dw (kernelE - kernel) / 512 + 1
	.kernel_addr		dw 0x0900
	.kernel_loc			dw 0x9000

failed:
.failed_loop:
	jmp .failed_loop

times 510 - ($ - $$) db 0x0
dw 0xAA55

second_stage: incbin "{data['second_stage_binary']}"
second_stageE:

kernel: incbin "{data['kernel_binary']}"
kernelE:
    """

    # Makefile :D
    FLAGS = "{FLAGS}"
    makefile = f""".PHONY: run
.PHONY: clean
.PHONY: build

FLAGS = -masm=intel -O1 -Wno-error -c -nostdinc -nostdlib -fno-builtin -fno-stack-protector -ffreestanding -m32
run: build
	@qemu-system-i386 -drive format=raw,file=bin/OS.image

build:
	@nasm protocol/protocol_util.asm -f elf32 -o bin/protocol_util.o
	@gcc ${FLAGS} -o bin/{second_stage_source_code_file_o} {second_stage_source_code_file}
	@gcc ${FLAGS} -o bin/{kernel_source_code_file_o} {kernel_source_code_file}
	@nasm boot/boot.asm -f bin -o bin/bootloader.bin
	@ld -m elf_i386 -TOS/linker.ld -nostdlib --nmagic -o bin/boot.out bin/{second_stage_source_code_file_o} bin/protocol_util.o
	@ld -m elf_i386 -TOS/kernel.ld -nostdlib --nmagic -o bin/kernel.out bin/{kernel_source_code_file_o} bin/protocol_util.o
	@objcopy -O binary bin/boot.out bin/boot.bin
	@objcopy -O binary bin/kernel.out bin/kernel.bin
	@cat bin/bootloader.bin bin/boot.bin bin/kernel.bin > bin/OS.image

clean:
	@rm -rf bin/*.out
	@rm -rf bin/*.o
	@rm -rf bin/*.bin

    """

    # Write the code to the bootloader file so our OS can load it :D
    file = open("../boot/boot.asm", "w")
    file.write(boot)
    file.close()

    file = open("../Makefile", 'w')
    file.write(makefile)
    file.close()