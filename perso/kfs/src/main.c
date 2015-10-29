#include	"main.h"
#include	"vga.h"
#include	"serial_port.h"

static void do_printk(unsigned int row, unsigned int col, const char *string)
{
	printk_on_screen(row, col, string);
	printk_on_serial_port(string);
}

void printk(const char *string)
{
	do_printk(0, 0, string);
}

static void print_hello_world(void)
{
	int cursor = 0;
	char c;

	init_serial();
	printk("hello world !");
	while (1) {
		c = read_serial();
		print_char_on_screen(1, cursor++, c, 0x14);
		write_serial(c);
	}
}

int main(unsigned int magic_number, const void *multiboot_info)
{
	if (magic_number != MAGIC_NUMBER) {
		printk("Invalid magic number.");
		return -1;
	}
	print_hello_world();
	return 0;
}
