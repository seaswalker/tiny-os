# include "init.h"
# include "kernel/print.h"

void init_all() {
    put_str("init_all.\n");
    idt_init();
}