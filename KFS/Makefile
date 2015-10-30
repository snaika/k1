NAME		=	kernel

BOOTSTRAP	=	src/crt0.S
SRC		=	src/main.c	\
			src/serial.c	\
			src/utils.c	\
			src/vga.c

OBJS		=	$(SRC:.c=.o) $(BOOTSTRAP:.S=.o)

LINKER_SCRIPT	=	kfs.ld

CFLAGS	+= -nostdinc -fno-builtin -m32
CFLAGS	+= -nostdlib -fno-stack-protector
CFLAGS	+= -Wall -std=gnu11 -ggdb3
CFLAGS	+= -iquote inc/

LDFLAGS	+= -nostdlib -m32 -Wl,--build-id=none

CC	= gcc

QEMU_OPTIONS = -serial stdio

all: $(NAME)

.S.o:
	$(CC) -c $(CFLAGS) $< -o $@

$(NAME) : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -T $(LINKER_SCRIPT) -o $(NAME)

boot: all
	qemu-system-x86_64 -kernel ./${NAME} $(QEMU_OPTIONS)

debug: all
	qemu-system-x86_64 -kernel ./${NAME} -s -S $(QEMU_OPTIONS) &
	gdb ./${NAME} -ex "target remote localhost:1234" -ex "b main" -ex "c"

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all boot debug clean fclean re
