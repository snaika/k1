#pragma once

#include "utils.h"

#define VGA_CRTC_ADDRESS	0x3D4
#define VGA_CRTC_DATA		0x3D5

#define VGA_CURSOR_HIGH		0x0E
#define VGA_CURSOR_LOW		0x0F

void	vga_init();
void	vga_clear_screen();
void	vga_putchar(char c);
void	vga_print(const char *str);
