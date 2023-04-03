.DEFAULT_GOAL := all

PLATFORM = riscv64-unknown-elf-

GCC = ${PLATFORM}gcc
GCC_FLAGS = -nostdlib -fno-builtin -march=rv64gcv -g -Wall

QEMU = qemu-system-riscv64
QEMU_FLAGS = -machine virt -bios none -m 128M -nographic

ASM_FILE = boot.s

C_FILE = kernel.c \
         uart.c

OBJ_FILE = ${ASM_FILE:.s=.o}

all: os.elf

os.elf: ${OBJ_FILE}
	${GCC} ${GCC_FLAGS} -o $@ $^

%.o: %.s
	${GCC} ${GCC_FLAGS} -c -o $@ $^

run: all
	${QEMU} ${QEMU_FLAGS} -kernel os.elf

.Phony: clean
clean:
	rm *.o
	rm *.elf
