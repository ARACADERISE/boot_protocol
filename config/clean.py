import subprocess
import os

subprocess.run('rm -rf bin/*.bin && rm -rf bin/*.o && rm -rf bin/*.out', shell=True, cwd=os.getcwd())
subprocess.run('rm -rf bin/*.image', shell=True, cwd=os.getcwd())
subprocess.run('rm -rf boot/boot.s', shell=True, cwd=os.getcwd())

with open('Makefile', 'w') as f:
    f.write('''.PHONY: build

build:
	@cd config && make build
    ''')
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

    jmp 0x8:0x%lx

%%include "boot/gdt.s"
    ''')
    f.close()