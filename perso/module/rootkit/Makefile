MAKE		=	make

obj-m		+=	rootkit.o

RM		=	rm -f

all	:
		gcc test.c -W -o test
		$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean	:
		$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
		$(RM) rootkit.h *~ test
