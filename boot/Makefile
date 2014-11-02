CC		= gcc
CFLAGS	= -m32 -O2 -Wall -Werror -fno-builtin -I./include
ASFLAGS	= -m32

CFILES	= $(shell find src/ -name "*.c")
SFILES	= $(shell find src/ -name "*.S")
OBJS	= $(CFILES:.c=.o)

loader: $(OBJS) src/start.o
	ld -o loader -e start -Ttext 0x100000 -m elf_i386 src/start.o $(OBJS)

clean:
	-rm -f loader $(OBJS) 2> /del/null
