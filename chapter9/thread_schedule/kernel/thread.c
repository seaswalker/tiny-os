# include "thread.h"
# include "stdint.h"
# include "string.h"
# include "global.h"
# include "memory.h"

# define PAGE_SIZE 4096

static void kernel_thread(thread_func* function, void* func_args);

static void kernel_thread(thread_func* function, void* func_args) {
    function(func_args);
}

/**
 * 初始化线程栈.
 */ 
void thread_create(struct task_struct* pthread, thread_func function, void* func_args) {
    pthread->self_kstack -= sizeof(struct intr_stack);
    pthread->self_kstack -= sizeof(struct thread_stack);

    struct thread_stack* kthread_stack = (struct thread_stack*) pthread->self_kstack;
    kthread_stack->eip = kernel_thread;
    kthread_stack->function = function;
    kthread_stack->func_args = func_args;
    kthread_stack->ebp = kthread_stack->ebx = kthread_stack->edi = kthread_stack->esi = 0;
}

/**
 * 初始化线程基本信息.
 */ 
void init_thread(struct task_struct* pthread, char* name, int prio) {
    memset(pthread, 0, sizeof(*pthread));
    strcpy(pthread->name, name);
    pthread->status = TASK_RUNNING;
    pthread->priority = prio;
    // PCB所在物理页的顶端地址
    pthread->self_kstack = (uint32_t*) ((uint32_t) pthread + PAGE_SIZE);
    pthread->stack_magic = 0x77777777;
}

/**
 * 创建线程.
 */ 
struct task_struct* thread_start(char* name, int prio, thread_func function, void* func_args) {
    struct task_struct* thread = get_kernel_pages(1);

    init_thread(thread, name, prio);
    thread_create(thread, function, func_args);

    asm volatile ("movl %0, %%esp; pop %%ebp; pop %%ebx; pop %%edi; pop %%esi; ret" : : "g" (thread->self_kstack) : "memory");
    return thread;
}