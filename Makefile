.PHONY: build

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
	@cd config && make build
