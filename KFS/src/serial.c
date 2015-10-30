#include "utils.h"
#include "serial.h"

void	serial_init()
{
  // Désactive les interruptions
  outb(UART_ITR_ENABLE, 0x00);
  // Mode fifo
  outb(UART_FIFO_CONTROL, 0xC7);

  // Vitesse
  outb(UART_LINE_CONTROL, 0x80);
  outb(UART_DL_LS, 0x03);
  outb(UART_DL_MS, 0x0);
  outb(UART_LINE_CONTROL, 0x0);

  // Configuration taille, bits stop & parité
  outb(UART_LINE_CONTROL, 0x03 | 0x0 << 2 | 0x0 << 3);

  // Options du modem
  outb(UART_MODEM_CONTROL, 0x3);
}

void	serial_putchar(char c)
{
  while (UART_TRANSMITTER_HOLDING_EMPTY == 0);
  while (UART_CLEAR_TO_SEND == 0);
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
