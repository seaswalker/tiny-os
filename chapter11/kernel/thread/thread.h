# ifndef _THREAD_H
# define _THREAD_H

# include "stdint.h"
# include "kernel/list.h"
# include "memory.h"

/**
 * 自定义通用函数类型.
 */ 
typedef void thread_func(void*);

# define PAGE_SIZE 4096

/**
 * 线程状态.
 */ 
enum task_status {
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_WAITTING,
    TASK_HANGING,
    TASK_DIED
};

/**
 * 中断栈.
 */
struct intr_stack {
    uint32_t vec_no;
    uint32_t edi;  
    uint32_t esi;  
    uint32_t ebp;  
    uint32_t esp_dummy;  
    uint32_t ebx;  
    uint32_t edx;  
    uint32_t ecx;  
    uint32_t eax;  
    uint32_t gs;  
    uint32_t fs;  
    uint32_t es;  
    uint32_t ds;

    // 下面的属性由CPU从低特权级进入高特权级时压入
    uint32_t err_code;  
    void (*eip) (void);
    uint32_t cs;
    uint32_t eflags;
    void* esp;
    uint32_t ss;
};

struct thread_stack {
    uint32_t ebp;
    uint32_t ebx;
    uint32_t edi;
    uint32_t esi;

    // 第一次执行时指向待调用的函数kernel_thread，其它时候指向switch_to的返回地址.
    void (*eip) (thread_func* func, void* func_args);

    void (*unused_retaddr);
    thread_func* function;
    void* func_args;
};

/**
 * PCB，进程或线程的控制块.
 */ 
struct task_struct {
    // 内核栈
    uint32_t* self_kstack;
    enum task_status status;
    char name[16];
    uint8_t priority;
    // 当前线程可以占用的CPU嘀嗒数
    uint8_t ticks;
    // 此任务占用的总嘀嗒数
    uint32_t elaspsed_ticks;
    // 可执行队列节点
    struct list_elem general_tag;
    // 所有不可运行线程队列节点
    struct list_elem all_list_tag;
    uint32_t* pgdir;
    struct virtual_addr userprog_addr;
    uint32_t stack_magic;
};

struct task_struct* running_thread();
void thread_create(struct task_struct* pthread, thread_func function, void* func_args);
void init_thread(struct task_struct* pthread, char* name, int prio);
struct task_struct* thread_start(char* name, int prio, thread_func function, void* func_args);
void schedule();
void thread_init();
void thread_block(enum task_status status);
void thread_unblock(struct task_struct* pthread);

# endif