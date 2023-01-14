import subprocess
import os

subprocess.run('rm -rf bin/*.bin && rm -rf bin/*.o && rm -rf bin/*.out', shell=True, cwd=os.getcwd())
subprocess.run('rm -rf bin/*.image', shell=True, cwd=os.getcwd())
subprocess.run('rm -rf boot/boot.s', shell=True, cwd=os.getcwd())

with open('Makefile', 'w') as f:
    f.write('''.PHONY: build

FLAGS = -masm=intel -O1 -Wno-error -c -nostdinc -nostdlib -fno-builtin -fno-stack-protector -ffreestanding -m32
build:
	@./config/scripts/quick_edit
	@nasm protocol/protocol_util.s -f elf32 -o ../bin/protocol_util.o
	@gcc ${FLAGS} -o ../bin/second_stage.o ../main.c
	@gcc ${FLAGS} -o ../bin/kernel.o ../kernel.c
	@ld -m elf_i386 -Tlinker/linker.ld -nostdlib --nmagic -o ../bin/boot.out ../bin/second_stage.o ../bin/protocol_util.o
	@ld -m elf_i386 -Tlinker/kernel.ld -nostdlib --nmagic -o ../bin/kernel.out ../bin/kernel.o ../bin/protocol_util.o
	@objcopy -O binary ../bin/boot.out ../bin/second_stage.bin
	@objcopy -O binary ../bin/kernel.out ../bin/kernel.bin
	@cd config && make build''')
    f.close()

with open('protocol/gdt/gdt_ideals.s', 'w') as f:
    f.write('''use16
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
    ''')
    f.close()

# Delete linker scripts. They will be recreated anyway
subprocess.run('rm -rf linker/linker.ld', shell=True, cwd=os.getcwd())
subprocess.run('rm -rf linker/kernel.ld', shell=True, cwd=os.getcwd())