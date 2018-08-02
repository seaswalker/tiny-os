# ifndef _KERNEL_MEMORY_H
# define _KERNEL_MEMORY_H

# include "stdint.h"
# include "bitmap.h"

struct virtual_addr {
    struct bitmap vaddr_bitmap;
    // 虚拟内存的起始地址
    uint32_t vaddr_start;
};

extern struct pool kernel_pool, user_pool;
void mem_init(void);

# endif