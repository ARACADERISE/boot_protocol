.PHONY: build
.PHONY: mbr_partition_table
.PHONY: higher_half_kernel_program
.PHONY: tools
.PHONY: first_half
.PHONY: famp_config

FLAGS = -masm=intel -O1 -Wno-error -c -nostdinc -nostdlib -fno-builtin -fno-stack-protector -ffreestanding -m32

build: mbr_partition_table higher_half_kernel_program first_half famp_config
	@python3 config/py_scripts/move.py move_disk_image
	@gcc YamlParser/parser.c YamlParser/data.c YamlParser/lexer.c config/format_disk_image.c -o tools_bin/format_disk_image.o
	@python3 config/py_scripts/move.py

first_half:
	@chmod +x config/scripts/*
	@nasm protocol/protocol_util.s -f elf32 -o ../bin/protocol_util.o
	@gcc ${FLAGS} -o bin/second_stage.o boot/second_stage.c
	@gcc ${FLAGS} -o ../bin/kernel.o ../kernel.c
	@ld -m elf_i386 -Tlinker/linker.ld -nostdlib --nmagic -o bin/boot.out bin/second_stage.o ../bin/protocol_util.o
	@ld -m elf_i386 -Tlinker/kernel.ld -nostdlib --nmagic -o ../bin/kernel.out ../bin/kernel.o ../bin/protocol_util.o
	@objcopy -O binary bin/boot.out bin/second_stage.bin
	@objcopy -O binary ../bin/kernel.out ../bin/kernel.bin
	@./bin/format.o bin/second_stage.bin --second_stage
	@./bin/format.o ../bin/kernel.bin --kernel

famp_config:
	@cd config && make build
	@nasm boot/boot.s -f bin -o ../bin/boot.bin
	@cd config && make eve

tools:
	@gcc YamlParser/parser.c YamlParser/data.c YamlParser/lexer.c config/format_disk_image.c -o tools_bin/format_disk_image.o
	@python3 config/py_scripts/move.py

remove_tools:
	@python3 config/py_scripts/remove.py

purge_tools:
	@python3 config/py_scripts/remove.py --purge

mbr_partition_table:
	@gcc config/format.c -o bin/format.o
	@nasm boot/partition_util.s -f elf32 -o bin/partition_util.o
	@gcc ${FLAGS} -o bin/mbr_partition_table.o -c boot/mbr_partition_table.c
	@ld -m elf_i386 -Tboot/mbr_partition_table.ld -nostdlib --nmagic -o bin/mbr_partition_table.out bin/mbr_partition_table.o bin/partition_util.o
	@objcopy -O binary bin/mbr_partition_table.out bin/mbr_partition_table.bin
	@./bin/format.o bin/mbr_partition_table.bin --jpad
 
higher_half_kernel_program:
	@gcc ${FLAGS} -o bin/higher_half_kernel.o -c boot/higher_half_kernel.c
	@ld -m elf_i386 -Tboot/higher_half_kernel.ld -nostdlib --nmagic -o bin/higher_half_kernel.out bin/higher_half_kernel.o
	@objcopy -O binary bin/higher_half_kernel.out bin/higher_half_kernel.bin
	@./bin/format.o bin/higher_half_kernel.bin --jpad

pclean:
	@./config/scripts/clean purge
	@python3 config/py_scripts/remove.py --purge