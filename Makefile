.PHONY: run
.PHONY: clean
.PHONY: build

FLAGS = -masm=intel -O1 -Wno-error -c -nostdinc -nostdlib -fno-builtin -fno-stack-protector -ffreestanding -m32
run: build
	@qemu-system-i386 -drive format=raw,file=bin/OS.image

build:
	@nasm protocol/protocol_util.asm -f elf32 -o bin/protocol_util.o
	@gcc ${FLAGS} -o bin/main.o OS/main.c
	@gcc ${FLAGS} -o bin/kernel.o OS/kernel.c
	@ld -m elf_i386 -TOS/linker.ld -nostdlib --nmagic -o bin/boot.out bin/main.o bin/protocol_util.o
	@ld -m elf_i386 -TOS/kernel.ld -nostdlib --nmagic -o bin/kernel.out bin/kernel.o bin/protocol_util.o
	@objcopy -O binary bin/boot.out bin/boot.bin
	@objcopy -O binary bin/kernel.out bin/kernel.bin
	@nasm boot/boot.asm -f bin -o bin/bootloader.bin
	@cat bin/bootloader.bin bin/boot.bin bin/kernel.bin > bin/OS.image

clean:
	@rm -rf bin/*.out
	@rm -rf bin/*.o
	@rm -rf bin/*.bin

    