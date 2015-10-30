#include "vga.h"

static unsigned char *video = (unsigned char*)0xB8000;
static unsigned int cursor_x = 0;
static unsigned int cursor_y = 0;
static char vga_color = 0 << 4 | 7;

void	vga_init()
{
  vga_set_internal_cursor();
  vga_clear_screen();
}

void	vga_set_internal_cursor()
{
  unsigned pos;

  pos = cursor_y * 80 + cursor_x;
  outb(VGA_CRTC_ADDRESS, VGA_CURSOR_HIGH);
  outb(VGA_CRTC_DATA, pos >> 8);
  outb(VGA_CRTC_ADDRESS, VGA_CURSOR_LOW);
  outb(VGA_CRTC_DATA, pos);
}

void	vga_putchar_at(char c, int x, int y)
{
  video[(y * 80 + x) * 2] = c;
  video[(y * 80 + x) * 2 + 1] = vga_color;
}

void	vga_clear_screen()
{
  int x, y;

  for (x = 0; x < 80; x++)
    for (y = 0; y < 25; y++)
      vga_putchar_at(' ', x, y);
  
  cursor_x = 0;
  cursor_y = 0;
  vga_set_internal_cursor();
}

void	vga_scroll()
{
  int lines_past = cursor_y - 24;
  int i;
  int j;
  
  if (lines_past < 1)
    return;

  memcpy(video, video + lines_past * 80 * 2, 80 * (25 - lines_past) * 2);
  for (i = 25 - lines_past; i < 25; i++)
    {
      for (j = 0; j < 80; j++)
	vga_putchar_at(' ', j, i);
    }
  cursor_y = 24;
  vga_set_internal_cursor();
}

void	vga_putchar(char c)
{
  if (c == '\n')
    {
      cursor_x = 0;
      cursor_y++;
      vga_scroll();
      vga_set_internal_cursor();
      return;
    }
  vga_putchar_at(c, cursor_x, cursor_y);
  cursor_x++;
  if (cursor_x > 79)
    {
      cursor_x = 0;
      cursor_y++;
    }
  vga_scroll();
  vga_set_internal_cursor();
}

void	vga_print(const char *str)
{
  size_t i = 0;

  while (str[i] != '\0')
    {
      vga_putchar(str[i]);
      i++;
    }
}
