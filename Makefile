.PHONY: build
.PHONY: mbr_partition_table
.PHONY: higher_half_kernel_program

FLAGS = -masm=intel -O1 -Wno-error -c -nostdinc -nostdlib -fno-builtin -fno-stack-protector -ffreestanding -m32
build: mbr_partition_table higher_half_kernel_program
	@chmod +x config/scripts/*
	@nasm protocol/protocol_util.s -f elf32 -o ../bin/protocol_util.o
	@gcc ${FLAGS} -o ../bin/second_stage.o ../main.c
	@gcc ${FLAGS} -o ../bin/kernel.o ../kernel.c
	@ld -m elf_i386 -Tlinker/linker.ld -nostdlib --nmagic -o ../bin/boot.out ../bin/second_stage.o ../bin/protocol_util.o
	@ld -m elf_i386 -Tlinker/kernel.ld -nostdlib --nmagic -o ../bin/kernel.out ../bin/kernel.o ../bin/protocol_util.o
	@objcopy -O binary ../bin/boot.out ../bin/second_stage.bin
	@objcopy -O binary ../bin/kernel.out ../bin/kernel.bin
	@./bin/format.o ../bin/second_stage.bin --second_stage
	@./bin/format.o ../bin/kernel.bin --kernel
	@cd config && make build
	
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