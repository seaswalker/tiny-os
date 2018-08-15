# include "kernel/print.h"
# include "init.h"
# include "interrupt.h"

int main(void) {
    // 这里不能使用console_put_str，因为还没有初始化
    put_str("I am kernel.\n");
    init_all();

    intr_enable();

    while (1);

    return 0;
}