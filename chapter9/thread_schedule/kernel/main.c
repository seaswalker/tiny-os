# include "kernel/print.h"
# include "init.h"
# include "thread.h"

void k_thread_function(void*);

int main(void) {
    put_str("I am kernel.\n");
    init_all();

    thread_start("k_thread_1", 31, k_thread_function, "skywalker ");

    while (1);
    return 0;
}

void k_thread_function(void* args) {
    // 这里必须是死循环，否则执行流并不会返回到main函数，所以CPU将会放飞自我，出发6号未知操作码异常
    while (1) {
        put_str((char*) args);
    }
}