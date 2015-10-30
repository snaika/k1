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
  int i = 0;
  
  init();
  while (1)
    {
      i++;

      if (i % 20000000 == 0)
	printk("Hello world !\n");
    }
  return 0;
}
