# include "memory.h"
# include "stdint.h"
# include "kernel/print.h"

# define PAGE_SIZE 4096

// 位图地址
# define MEM_BITMAP_BASE 0xc009a00

// 跳过低端1MB内存，中间10为代表页表项偏移，即0x100，即256 * 4KB = 1MB
# define K_HEAD_START 0xc0100000

struct pool {
    struct bitmap poool_bitmap;
    uint32_t phy_addr_start;
    uint32_t pool_size;
};

struct pool kernel_pool, user_pool;
struct virtual_addr kernel_addr;

/**
 * 初始化内存池.
 */ 
static void mem_pool_init(uint32_t all_memory) {
    put_str("Start init Memory pool...\n");
    uint32_t page_table_size = PAGE_SIZE * 256;

    
}