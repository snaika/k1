obj-m	+=	NewOpen.o

all: NewOpen.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f NewOpen.c

NewOpen.c:
	./getSysCallTable.sh
