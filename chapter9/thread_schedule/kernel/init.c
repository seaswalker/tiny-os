# include "init.h"
# include "kernel/print.h"
# include "timer.h"
# include "memory.h"
# include "thread.h"

void init_all() {
    put_str("init_all.\n");
    idt_init();
    mem_init();
    thread_init();
    timer_init();
}