# include "kernel/print.h"

int main(void) {
     put_char('k');
    put_char('e');
    put_char('r');
    put_char('n');
    put_char('e');
    put_char('l');
    put_char('\n');
    put_char('1');
    put_char('2');
    put_char('\b');
    put_char('3');
    put_char('\n');
    put_str("I am kernel!\n");

    put_int(7);
    put_char('\n');
    put_int(0x7c00);

    while (1);
    return 0;
}