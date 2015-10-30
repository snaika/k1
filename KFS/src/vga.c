#include "vga.h"

static unsigned char *video = (unsigned char*)0xB8000;
static unsigned int cursor_col = 0;
static unsigned int cursor_row = 0;
static void vga_set_internal_cursor();
static void vga_scroll();
static void vga_putchar_at(char c, int col, int row);

static void vga_set_internal_cursor()
{
  unsigned int pos;

  pos = cursor_row * 80 + cursor_col;
  outb(VGA_CRTC_ADDRESS, VGA_CURSOR_HIGH);
  outb(VGA_CRTC_DATA, pos >> 8);
  outb(VGA_CRTC_ADDRESS, VGA_CURSOR_LOW);
  outb(VGA_CRTC_DATA, pos);
}

static void vga_scroll()
{
  int lines_past = cursor_row - 24;
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
  cursor_row = 24;
  vga_set_internal_cursor();
}

static void vga_putchar_at(char c, int col, int row)
{
  video[(row * 80 + col) * 2] = c;
  video[(row * 80 + col) * 2 + 1] = 7;
  vga_set_internal_cursor();
}

void vga_init()
{
  vga_clear_screen();
}

void vga_clear_screen()
{
  int cols, rows;

  for (cols = 0; cols < 80; cols++)
    for (rows = 0; rows < 25; rows++)
      vga_putchar_at(' ', cols, rows);
  
  cursor_col = 0;
  cursor_row = 0;
  vga_set_internal_cursor();
}

void vga_putchar(char c)
{
  if (c == '\n')
    {
      cursor_col = 0;
      cursor_row++;
      vga_scroll();
      vga_set_internal_cursor();
      return;
    }
  vga_putchar_at(c, cursor_col, cursor_row);
  cursor_col++;
  if (cursor_col > 79)
    {
      cursor_col = 0;
      cursor_row++;
    }
  vga_scroll();
  vga_set_internal_cursor();
}

void vga_print(const char *str)
{
  size_t i = 0;

  while (str[i] != '\0')
    {
      vga_putchar(str[i]);
      i++;
    }
}
