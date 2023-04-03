extern void uart_init();
extern void uart_put_str(char *s);

void start_kernel()
{
    uart_init();
    uart_put_str("Hello RVOS!\n");

    while (1);
}
