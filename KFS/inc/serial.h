#pragma once

#define UART_BASE	0x03F8

enum UART_INFO
  {
    UART_BUFFER         = UART_BASE + 0, // Tx | Rx | DLATCH_LOW
    UART_ITR_ENABLE     = UART_BASE + 1, // Activation des interruptions | DLATCH_HIGH
    UART_ITR_REGISTER   = UART_BASE + 2, // Read-only : Registre d'indentifications des interruptions
    UART_FIFO_CONTROL   = UART_BASE + 2, // Write-only :
    UART_LINE_CONTROL   = UART_BASE + 3,
    UART_MODEM_CONTROL  = UART_BASE + 4,
    UART_LINE_STATUS    = UART_BASE + 5, // Read-only
    UART_MODEM_STATUS   = UART_BASE + 6,
    UART_DL_LS          = UART_BASE + 0,
    UART_DL_MS          = UART_BASE + 1
  };

#define UART_DATA_READY_INDICATOR       (inb(UART_LINE_STATUS) & 0x01)
#define UART_TRANSMITTER_HOLDING_EMPTY  (inb(UART_LINE_STATUS) & 0x20)
#define UART_CLEAR_TO_SEND              (inb(UART_MODEM_STATUS) & 0x20)

void	serial_init();
void	serial_putchar(char c);
void	serial_write(const char *msg);
char	serial_read();
