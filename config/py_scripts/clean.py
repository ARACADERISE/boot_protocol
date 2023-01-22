import subprocess
import os
import yaml

yaml_data = None
with open('../boot.yaml', 'r') as file:
    yaml_data = yaml.full_load(file)
    file.close()

# Delete all binaries
subprocess.run(f'rm -rf ../{yaml_data["bin_folder"]}/*.bin', shell=True, cwd=os.getcwd())
subprocess.run(f'rm -rf ../{yaml_data["bin_folder"]}/*.o', shell=True, cwd=os.getcwd())
subprocess.run(f'rm -rf ../{yaml_data["bin_folder"]}/*.out', shell=True, cwd=os.getcwd())
subprocess.run(f'rm -rf ../{yaml_data["bin_folder"]}/*.image', shell=True, cwd=os.getcwd())
subprocess.run('rm -rf boot/boot.s', shell=True, cwd=os.getcwd())

# Delete all local binaries
subprocess.run('rm -rf bin/*.o', shell=True, cwd=os.getcwd())

flags = '{FLAGS}'

with open('Makefile', 'w') as f:
    f.write(f'''.PHONY: build

FLAGS = -masm=intel -O1 -Wno-error -c -nostdinc -nostdlib -fno-builtin -fno-stack-protector -ffreestanding -m32
build:
	@gcc config/format.c -o bin/format.o
	@nasm protocol/protocol_util.s -f elf32 -o ../bin/protocol_util.o
	@gcc ${flags} -o ../{yaml_data["second_stage_bin_o_filename"]} ../{yaml_data["second_stage_source_code_file"]}
	@gcc ${flags} -o ../{yaml_data["kernel_o_binary"]} ../{yaml_data["kernel_source_code_file"]}
	@ld -m elf_i386 -Tlinker/linker.ld -nostdlib --nmagic -o ../bin/boot.out ../{yaml_data["second_stage_bin_o_filename"]} ../bin/protocol_util.o
	@ld -m elf_i386 -Tlinker/kernel.ld -nostdlib --nmagic -o ../bin/kernel.out ../{yaml_data["kernel_o_binary"]} ../bin/protocol_util.o
	@objcopy -O binary ../bin/boot.out ../{yaml_data["second_stage_bin_filename"]}
	@objcopy -O binary ../bin/kernel.out ../{yaml_data["kernel_bin_filename"]}
	@cd config && make build''')
    f.close()