# ifndef _KERNEL_MEMORY_H
# define _KERNEL_MEMORY_H

# include "stdint.h"
# include "bitmap.h"

// 存在标志
# define PG_P_1 1
# define PG_P_0 0
// 只读
# define PG_RW_R 0
// 可写
# define PG_RW_W 2
// 系统级
# define PG_US_S 0
# define PG_US_U 4

/**
 * 内存池类型标志.
 */ 
enum pool_flags {
    // 内核类型
    PF_KERNEL = 1,
    PF_USER = 2
};

struct virtual_addr {
    struct bitmap vaddr_bitmap;
    // 虚拟内存的起始地址
    uint32_t vaddr_start;
};

extern struct pool kernel_pool, user_pool;

void mem_init(void);
void* get_kernel_pages(uint32_t page_count);
void* malloc_page(enum pool_flags pf, uint32_t page_count);
uint32_t addr_v2p(uint32_t vaddr);
void* get_a_page(enum pool_flags pf, uint32_t vaddr);
void* get_user_pages(uint32_t page_count);

# endif