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
    while (1);
    return 0;
}