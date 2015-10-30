#include "utils.h"

void *memcpy(void *destination, const void *source, size_t n)
{
  int i;
  for (i = 0; i < n; i++)
    {
      ((unsigned char *)destination)[i] = ((unsigned char *)source)[i];
    }
  return destination;
}

size_t strlen(const char *s)
{
  size_t i = 0;

  while (s[i] != '\0')
    i++;
  return i;
}

void outb(unsigned short int port, unsigned char value)
{
  __asm__ volatile("outb %0, %1\n\t"
		   :
		   : "a" (value), "d" (port));
}

unsigned char inb(unsigned short int port)
{
  char ret;

  __asm__ volatile ("inb %1, %0\n\t"
		    : "=a" (ret)
		    : "d" (port));
  return ret;
}
