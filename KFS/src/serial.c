#include "utils.h"
#include "serial.h"

void	serial_init()
{
  // Désactive les interruptions
  outb(UART_ITR_ENABLE, 0x00);

  outb(UART_LINE_CONTROL, 0x80);
  outb(UART_BUFFER, 0x03);
  outb(UART_DL_LS, 0x00);
  outb(UART_LINE_CONTROL, 0x03);
}

void	serial_putchar(char c)
{
  while (UART_TRANSMITTER_HOLDING_EMPTY == 0);
  outb(UART_BUFFER, c);
}

void	serial_write(const char *msg)
{
  for (int i = 0; msg[i] != '\0'; i++)
    serial_putchar(msg[i]);
}

char	serial_read()
{
  while (UART_DATA_READY_INDICATOR == 0);
  return inb(UART_BUFFER);
}
