# include "kernel/print.h"
# include "init.h"
# include "debug.h"
# include "memory.h"

int main(void) {
    put_str("I am kernel.\n");
    init_all();
    put_str("Init done.\n");
    
    void* vaddr = get_kernel_pages(3);
    put_str("\nKernel memory virtual page start address: ");
    put_int((uint32_t) vaddr);
    put_char('\n');

    while (1);
    return 0;
}