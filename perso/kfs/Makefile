NAME		=	kfs

BOOTSTRAP	=	src/crt0.S

SRCS		=	src/main.c		\
			src/serial_port.c	\
			src/vga.c		\
			src/tools.c

LINKER		=	kfs.ld

OBJS		=	$(SRCS:.c=.o) $(BOOTSTRAP:.S=.o)

CFLAGS		+=	-nostdinc
CFLAGS		+=	-fno-builtin
CFLAGS		+=	-m32
CFLAGS		+=	-nostdlib
CFLAGS		+=	-fno-stack-protector
CFLAGS		+=	-g -Iinc/

LDFLAGS		+=	-nostdlib -m32 -Wl,--build-id=none

CC		=	gcc

RM		=	rm -f

QEMU		=	qemu-system-x86_64

QEMU_PARAMS	=	-serial stdio

all		:	$(NAME)

.S.o		:
			$(CC) -c $(CFLAGS) $< -o $@

boot		:
			$(QEMU) -kernel $(NAME) $(QEMU_PARAMS)

debug		:	
			$(QEMU) -kernel $(NAME) $(QEMU_PARAMS) -s -S

$(NAME)		:	$(OBJS)
			$(CC) $(OBJS) -T $(LINKER) -o $(NAME) $(LDFLAGS)

clean		:
			$(RM) $(OBJS)
			$(RM) *~ src/*~ inc/*~

fclean		:	clean
			$(RM) $(NAME)

re		:	fclean all

.PHONY		:	all clean fclean re
