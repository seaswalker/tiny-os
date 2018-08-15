# include "sync.h"
# include "interrupt.h"
# include "debug.h"

void semaphore_init(struct semaphore* psem, uint8_t value) {
    psem->value = value;
    list_init(&psem->waiters);
}

void lock_init(struct lock* lock) {
    lock->holder = NULL;
    lock->holder_repeat_num = 0;
    semaphore_init(&lock->semaphore, 1);
}

void semaphore_down(struct semaphore* psem) {
    enum intr_status old_status = intr_disable();

    while (psem->value == 0) {
        struct task_struct* cur = running_thread();
        ASSERT(!list_find(&psem->waiters, &cur->general_tag));
        list_append(&psem->waiters, &cur->general_tag);
        thread_block(TASK_BLOCKED);
    }

    psem->value--;
    ASSERT(psem->value == 0);
    intr_set_status(old_status);
}

void semaphore_up(struct semaphore* psem) {
     enum intr_status old_status = intr_disable();
     ASSERT(psem->value == 0);

     if (!list_empty(&psem->waiters)) {
         struct task_struct* waiter = elem2entry(struct task_struct, general_tag, list_pop(&psem->waiters));
         thread_unblock(waiter);
     }

     psem->value++;
     ASSERT(psem->value == 1);
     intr_set_status(old_status);
}

/**
 * 申请锁.
 */ 
void lock_acquire(struct lock* plock) {
    struct task_struct* cur = running_thread();
    if (plock->holder != cur) {
        semaphore_down(&plock->semaphore);
        plock->holder = cur;
        ASSERT(plock->holder_repeat_num == 0);
        plock->holder_repeat_num = 1;
    } else {
        plock->holder_repeat_num++;
    }
}

/**
 * 锁释放.
 */ 
void lock_release(struct lock* plock) {
    ASSERT(plock->holder == running_thread());

    if (plock->holder_repeat_num > 1) {
        plock->holder_repeat_num--;
        return;
    }

    ASSERT(plock->holder_repeat_num == 1);

    plock->holder = NULL;
    plock->holder_repeat_num = 0;
    semaphore_up(&plock->semaphore);
}