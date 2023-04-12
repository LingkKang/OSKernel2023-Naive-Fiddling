extern void uart_init();
extern void uart_put_str(char *s);
extern void test_print();

void start_kernel()
{
    uart_init();
    test_print();

    while (1);
}
