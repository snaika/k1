#include "vga.h"
#include "serial.h"

// Not a standard main, but i want -Wall
#pragma GCC diagnostic ignored "-Wmain" 

void init();
void printk(const char *msg);

void init()
{
  vga_init();
  serial_init();
}

void printk(const char *msg)
{
  vga_print(msg);
  serial_write(msg);
}

int main(unsigned int magic_number, const void *multiboot_info)
{
  init();
  printk("Hello world !\n");
  while (1)
    {
      char c = serial_read();
      char str[2];
      str[0] = c;
      str[1] = '\0';
      if (strlen(str) > 0)
	printk(str);
    }
  return 0;
}
