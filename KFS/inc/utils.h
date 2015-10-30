#pragma once

#define size_t unsigned int

unsigned char	inb(unsigned short int port);
void		outb(unsigned short int port, unsigned char value);
size_t		strlen(const char *s);
void		*memcpy(void *destination, const void *source, size_t n);
