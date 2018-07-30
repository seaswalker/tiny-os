# include "kernel/print.h"
# include "init.h"
# include "debug.h"

int main(void) {
    put_str("I am kernel.\n");
    init_all();
    put_str("Init finished.\n");

    ASSERT(1 == 2);
    
    while (1);
    return 0;
}