# include "process.h"
# include "interrupt.h"
# include "memory.h"
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
    memcpy((uint32_t*) ((uint32_t) page_dir_vaddr + 0x300 * 4), (uint32_t*) (0xfffff000 + 0x300 * 4), 1024);

    // 设置最后一项页表的地址为页目录地址
    uint32_t new_page_dir_phy_addr = addr_v2p((uint32_t) page_dir_vaddr);
    page_dir_vaddr[1023] = (new_page_dir_phy_addr | PG_US_U | PG_RW_W | PG_P_1);
    
    return page_dir_vaddr;
}

/**
 * 为用户进程设置其单独的虚拟地址池.
 */ 
void create_user_vaddr_bitmap(struct task_struct* user_process) {
    user_process->userprog_addr.vaddr_start = USER_VADDR_START;
    // 0xc0000000是内核虚拟地址起始处
    uint32_t bitmap_page_count = DIV_ROUND_UP((0xc0000000 - USER_VADDR_START) / PAGE_SIZE / 8, PAGE_SIZE);

    user_process->userprog_addr.vaddr_bitmap.bits = get_kernel_pages(bitmap_page_count);
    user_process->userprog_addr.vaddr_bitmap.btmp_bytes_len = (0xc0000000 - USER_VADDR_START) / PAGE_SIZE / 8;

    bitmap_init(&user_process->userprog_addr.vaddr_bitmap);
}

/**
 * 创建用户进程.
 */ 
void process_execute(void* filename, char* name) {
    struct task_struct* pcb = get_kernel_pages(1);
    
    init_thread(pcb, name, default_prio);

    create_user_vaddr_bitmap(pcb);

    thread_create(pcb, start_process, filename);

    pcb->pgdir = create_page_dir();

    enum intr_status old_status = intr_disable();

    ASSERT(!list_find(&thread_ready_list, &pcb->general_tag));
    list_append(&thread_ready_list, &pcb->general_tag);

    ASSERT(!list_find(&thread_all_list, &pcb->all_list_tag));
    list_append(&thread_all_list, &pcb->all_list_tag);

    intr_set_status(old_status);
}