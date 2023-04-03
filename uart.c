#include "types.h"

// UART registers base address
#define UART0 0x10000000L

// Offset of each control registers from UART0
#define RHR 0 // Receive Holding Register (read mode)
#define THR 0 // Transmit Holding Register (write mode)
#define DLL 0 // LSB of Divisor Latch (write mode)
#define IER 1 // Interrupt Enable Register (write mode)
#define DLM 1 // MSB of Divisor Latch (write mode)
#define FCR 2 // FIFO Control Register (write mode)
#define ISR 2 // Interrupt Status Register (read mode)
#define LCR 3 // Line Control Register
#define MCR 4 // Modem Control Register
#define LSR 5 // Line Status Register
#define MSR 6 // Modem Status Register
#define SPR 7 // ScratchPad Register

volatile uint8 *uart_reg(int reg)
{
    return (volatile uint8 *)(UART0 + reg);
}

uint8 uart_read_reg(int reg)
{
    return *uart_reg(reg);
}

void uart_write_reg(int reg, uint8 val)
{
    *uart_reg(reg) = val;
    return;
}

void uart_init()
{
    // Disable interrupts
    uart_write_reg(IER, 0x00);

    // Enable Divisor Latch on LCR (Bit-7)
    uart_write_reg(LCR, uart_read_reg(LCR) | (1 << 7));
    // Set the baud rate 38.4k -> Divisor Latch 3 on 1.8432MHz clock
    uart_write_reg(DLL, 0x03); // LSB of Divisor Latch
    uart_write_reg(DLM, 0x00); // MSB of Divisor Latch

    // Set asynchronous data communication format
    uart_write_reg(LCR, 0x03);

    return;
}

void uart_put_char(char ch)
{
    while (1)
    { // Break Transmit Holding is empty on LSR (Bit-5)
        if (uart_read_reg(LSR) | (1 << 5))
            break;
    }
    uart_write_reg(THR, ch);
    return;
}

void uart_put_str(char *s)
{
    while (*s)
    {
        uart_put_char(*s);
        s++;
    }
    return;
}
