# include "interrupt.h"
# include "memory.h"
# include "process.h"
# include "global.h"
# include "debug.h"
# include "tss.h"
# include "thread/thread.h"

extern void intr_exit(void);

/**
 * 构建用户进程上下文信息.
 */ 
void start_process(void* filename) {
    void* function = filename;
    struct task_struct* cur = running_thread();

    // 指向中断栈(intr_stack的低端，即低地址处)，PCB布局回顾(从上到下表示内存地址从高到低)
    // 1. intr_stack
    // 2. thread_stack
    // PCB的属性self_kstack在线程创建完毕后指向thread_stack的最底部
    cur->self_kstack += sizeof(struct thread_stack);
    struct intr_stack* proc_stack = (struct intr_stack*) cur->self_kstack;

    proc_stack->edi = proc_stack->esi = proc_stack->ebp = proc_stack->esp_dummy = 0;
    proc_stack->ebx = proc_stack->edx = proc_stack->ecx = proc_stack->eax = proc_stack->gs = 0;

    // 为通过中断返回的方式进入3特权级做准备
    proc_stack->ds = proc_stack->es = proc_stack->fs = SELECTOR_U_DATA;
    proc_stack->eip = function;
    proc_stack->cs = SELECTOR_U_CODE;
    proc_stack->eflags = (EFLAGS_IOPL_0 | EFLAGS_MBS | EFLAGS_IF_1);
    proc_stack->esp = (void*) ((uint32_t) get_a_page(PF_USER, USER_STACK3_VADDR) + PAGE_SIZE);
    proc_stack->ss = SELECTOR_U_DATA;

    asm volatile ("movl %0, %%esp; jmp intr_exit" : : "g" (proc_stack) : "memory");
}

/**
 * 如果给定的PCB是进程，那么将其页表设置到CR3.
 */ 
void page_dir_activate(struct task_struct* pthread) {
    // 内核页表的物理地址，定义在boot.inc，进入保护模式时确定
    uint32_t page_phy_dir = 0x100000;

    if (pthread->pgdir != NULL) {
        // 进程，得到其页表地址
        page_phy_dir = addr_v2p((uint32_t) pthread->pgdir);
    }

    asm volatile ("movl %0, %%cr3" : : "r" (page_phy_dir) : "memory");
}

/**
 * 激活线程或进程的页表，更新tss中的esp0为进程的0特权级栈.
 */ 
void process_activate(struct task_struct* pthread) {
    ASSERT(pthread != NULL);

    page_dir_activate(pthread);

    if (pthread->pgdir != NULL) {
        update_tss_esp(pthread);
    }
}

/**
 * 为进程创建页目录表.
 */ 
uint32_t* create_page_dir(void) {
    uint32_t* page_dir_vaddr = get_kernel_pages(1);
    if (page_dir_vaddr == NULL) {
        PANIC("create_page_dir: get_kernel_pages failed!");
        return NULL;
    }

    // 将内核的页表复制到进程页目录项中，实现内核的共享
    memcpy((uint32_t*) ((uint32_t) page_dir_vaddr + 0x300 * 4), ((uint32_t*) 0xfffff000 + 0x300 * 4), 1024);

    uint32_t new_page_dir_phy_addr = addr_v2p((uint32_t) page_dir_vaddr);
    page_dir_vaddr[1023] = new_page_dir_phy_addr | PG_US_U | PG_RW_W | PG_P_1;
    return page_dir_vaddr;
}
