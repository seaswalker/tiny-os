# include "thread.h"
# include "stdint.h"
# include "string.h"
# include "global.h"
# include "memory.h"

# define PAGE_SIZE 4096

static void kernel_thread(thread_func* function, void* func_args) {
    function(func_args);
}

void thread_create(struct task_struct* pthread, thread_func function, void* func_args) {
    
}