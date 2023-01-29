import subprocess
import os
import yaml

# `boot.yaml` data
yaml_data = None

# Obtain the yaml data from `boot.yaml`
with open('../boot.yaml', 'r') as file:
    yaml_data = yaml.full_load(file)
    file.close()

# Delete all binaries
subprocess.run(f'rm -rf ../{yaml_data["bin_folder"]}/*.bin', shell=True, cwd=os.getcwd())
subprocess.run(f'rm -rf ../{yaml_data["bin_folder"]}/*.o', shell=True, cwd=os.getcwd())
subprocess.run(f'rm -rf ../{yaml_data["bin_folder"]}/*.out', shell=True, cwd=os.getcwd())
subprocess.run(f'rm -rf ../{yaml_data["bin_folder"]}/*.image', shell=True, cwd=os.getcwd())

# Delete the bootloader
subprocess.run('rm -rf boot/boot.s', shell=True, cwd=os.getcwd())

# Delete all local binaries
subprocess.run('rm -rf bin/*.o', shell=True, cwd=os.getcwd())

# This is needed so we can have "{FLAGS}" put in after `@gcc`
flags = '{FLAGS}'

# Rewrite the original Makefile
with open('Makefile', 'w') as f:
    f.write(f'''.PHONY: build

FLAGS = -masm=intel -O1 -Wno-error -c -nostdinc -nostdlib -fno-builtin -fno-stack-protector -ffreestanding -m32
build:
	@chmod +x config/scripts/*
	@gcc config/format.c -o bin/format.o
	@nasm protocol/protocol_util.s -f elf32 -o ../bin/protocol_util.o
	@gcc ${flags} -o ../bin/second_stage.o ../main.c
	@gcc ${flags} -o ../bin/kernel.o ../kernel.c
	@ld -m elf_i386 -Tlinker/linker.ld -nostdlib --nmagic -o ../bin/boot.out ../bin/second_stage.o ../bin/protocol_util.o
	@ld -m elf_i386 -Tlinker/kernel.ld -nostdlib --nmagic -o ../bin/kernel.out ../bin/kernel.o ../bin/protocol_util.o
	@objcopy -O binary ../bin/boot.out ../bin/second_stage.bin
	@objcopy -O binary ../bin/kernel.out ../bin/kernel.bin
	@cd config && make build''')
    f.close()
