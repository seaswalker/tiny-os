# include "init.h"
# include "kernel/print.h"
# include "timer.h"
# include "memory.h"

void init_all() {
    put_str("init_all.\n");
    idt_init();
    // 避免干扰内存实验
    timer_init();
    mem_init();
}