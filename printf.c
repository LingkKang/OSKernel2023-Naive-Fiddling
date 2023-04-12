#include <stdarg.h>
#include "types.h"

extern void uart_put_char(char ch);
extern void uart_put_str(char *s);

char digit[] = "0123456789abcdef";
char DIGIT[] = "0123456789ABCDEF";

void panic(char *e)
{
    uart_put_str("Panic: ");
    uart_put_str(e);
    uart_put_str("\n");
}

void print_int(int num, int base)
{
    uint8 reverse[32];
    int i;

    if (num < 0)
    {
        uart_put_char('-');
        num = -num;
    }

    for (i = 0; num != 0; i++)
    {
        reverse[i] = digit[num % base];
        num /= base;
    }

    while (i > 0)
        uart_put_char(reverse[--i]);

    return;
}

void print_hex(uint32 p, int cap)
{
    uint8 hex_bit;
    for (int i = 7; i >= 0; i--)
    {
        hex_bit = (p & (15 << i * 4)) >> i * 4;
        if (cap)
            uart_put_char(DIGIT[hex_bit]);
        else
            uart_put_char(digit[hex_bit]);
    }
    return;
}

void printf(char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    int i;
    uint8 c;
    for (i = 0; (c = format[i]) != 0; i++)
    {
        if (c != '%')
        {
            uart_put_char(c);
            continue;
        }

        c = format[++i];
        switch (c)
        {
        case 'c':
            uart_put_char(va_arg(ap, int));
            break;

        case 'd':
            print_int(va_arg(ap, int), 10);
            break;

        case 'p':
            uart_put_str("0x");
            print_hex(va_arg(ap, uint32), 0);
            break;

        case 's':
            uart_put_str(va_arg(ap, char *));
            break;
        
        case 'x':
            print_hex(va_arg(ap, uint32), 0);
            break;

        case 'X':
            print_hex(va_arg(ap, uint32), 1);
            break;

        case '%':
            uart_put_char('%');
            break;

        default:
            panic("unknown specifier");
            break;
        }
    }
    va_end(ap);

    return;
}

void test_print()
{
    // print_hex test
    print_hex(2, 0);
    uart_put_char('\n');
    print_hex(15, 0);
    uart_put_char('\n');
    print_hex(0xABCD98, 1);
    uart_put_char('\n');
    print_hex(123456789, 0); // 075b cd15
    uart_put_char('\n');
    print_hex(4294967295, 1); // FFFF FFFF
    uart_put_char('\n');

    // print_int test
    // base 10 part
    print_int(-235, 10);
    uart_put_char('\n');
    print_int(0xABCD, 10); // 43981
    uart_put_char('\n');
    print_int(987654321, 10);
    uart_put_char('\n');

    // printf test
    printf("Blah blah blah\n");
    printf("Today is %d %s %d\n", 2022, "April", 12);
    printf("Function puts is at %p\n", uart_put_str);
    printf("Number %d in hex is %x\n", 13579, 13579);
    printf("Test %c %% char...\n", 'a');
}
