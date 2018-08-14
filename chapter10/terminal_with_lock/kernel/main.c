# include "kernel/print.h"
# include "init.h"
# include "thread/thread.h"
# include "interrupt.h"
# include "console.h"

void k_thread_function_a(void*);
void k_thread_function_b(void*);

int main(void) {
    // 这里不能使用console_put_str，因为还没有初始化
    put_str("I am kernel.\n");
    init_all();

    thread_start("k_thread_a", 31, k_thread_function_a, "threadA ");
    thread_start("k_thread_b", 8, k_thread_function_b, "threadB ");

    intr_enable();

    while (1) {
        console_put_str("main ");
    }

    return 0;
}

void k_thread_function_a(void* args) {
    // 这里必须是死循环，否则执行流并不会返回到main函数，所以CPU将会放飞自我，出发6号未知操作码异常
    while (1) {
        console_put_str((char*) args);
    }
}

void k_thread_function_b(void* args) {
    while (1) {
        console_put_str((char*) args);
    }
}