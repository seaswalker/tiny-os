# ifndef _THREAD_SYNC_H
# define _THREAD_SYNC_H

# include "kernel/list.h"
# include "thread.h"
# include "stdint.h"

/**
 * 信号量.
 */ 
struct semaphore {
    uint8_t value;
    struct list waiters;
};

struct lock {
    struct task_struct* holder;
    struct semaphore semaphore;
    uint32_t holder_repeat_num;
};

void semaphore_init(struct semaphore* psem, uint8_t value);
void lock_init(struct lock* lock);
void semaphore_down(struct semaphore* psem);
void semaphore_up(struct semaphore* psem);
void lock_acquire(struct lock* plock);
void lock_release(struct lock* plock);

# endif