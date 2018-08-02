# include "memory.h"
# include "stdint.h"
# include "kernel/print.h"

# define PAGE_SIZE 4096

// 位图地址
# define MEM_BITMAP_BASE 0xc009a00

// 内核使用的起始虚拟地址
// 跳过低端1MB内存，中间10为代表页表项偏移，即0x100，即256 * 4KB = 1MB
# define K_HEAD_START 0xc0100000

static void printKernelPoolInfo(struct pool p);
static void printUserPoolInfo(struct pool p);

struct pool {
    struct bitmap pool_bitmap;
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

    // 页表(一级和二级)占用的内存大小，256的由来:
    // 一页的页目录，页目录的第0和第768项指向一个页表，此页表分配了低端1MB内存(其实此页表中也只是使用了256个表项)，
    // 剩余的254个页目录项实际没有分配对应的真实页表，但是需要为内核预留分配的空间
    uint32_t page_table_size = PAGE_SIZE * 256;

    // 已经使用的内存为: 低端1MB内存 + 现有的页表和页目录占据的空间
    uint32_t used_mem = (page_table_size + 0x100000);

    uint32_t free_mem = (all_memory - used_mem);
    uint16_t free_pages = free_mem / PAGE_SIZE;

    uint16_t kernel_free_pages = (free_pages >> 1);
    uint16_t user_free_pages = (free_pages - kernel_free_pages);

    // 内核空间bitmap长度(字节)，每一位代表一页
    uint32_t kernel_bitmap_length = kernel_free_pages / 8;
    uint32_t user_bitmap_length = user_free_pages / 8;

    // 内核内存池起始物理地址，注意内核的虚拟地址占据地址空间的顶端，但是实际映射的物理地址是在这里
    uint32_t kernel_pool_start = used_mem;
    uint32_t user_pool_start = (used_mem + kernel_free_pages * PAGE_SIZE);

    kernel_pool.phy_addr_start = kernel_pool_start;
    user_pool.phy_addr_start = user_pool_start;

    kernel_pool.pool_size = kernel_free_pages * PAGE_SIZE;
    user_pool.pool_size = user_free_pages * PAGE_SIZE;

    kernel_pool.pool_bitmap.btmp_bytes_len = kernel_bitmap_length;
    user_pool.pool_bitmap.btmp_bytes_len = user_bitmap_length;

    // 内核bitmap和user bitmap bit数组的起始地址
    kernel_pool.pool_bitmap.bits = (void*) MEM_BITMAP_BASE;
    user_pool.pool_bitmap.bits = (void*) (MEM_BITMAP_BASE + kernel_bitmap_length);

    printKernelPoolInfo(kernel_pool);
    printUserPoolInfo(user_pool);

    bitmap_init(&kernel_pool.pool_bitmap);
    bitmap_init(&user_pool.pool_bitmap);

    kernel_addr.vaddr_bitmap.btmp_bytes_len = kernel_bitmap_length;
    // 内核虚拟地址池仍然保存在低端内存以内
    kernel_addr.vaddr_bitmap.bits = (void*) (MEM_BITMAP_BASE + kernel_bitmap_length + user_bitmap_length);
    kernel_addr.vaddr_start = K_HEAD_START;

    bitmap_init(&kernel_addr.vaddr_bitmap);
    put_str("Init memory pool done.\n");
}

static void printKernelPoolInfo(struct pool p) {
    put_str("Kernel pool bitmap address: ");
    put_int(p.pool_bitmap.bits);
    put_str("; Kernel pool physical address: ");
    put_int(p.phy_addr_start);
    put_char('\n');
}

static void printUserPoolInfo(struct pool p) {
    put_str("User pool bitmap address: ");
    put_int(p.pool_bitmap.bits);
    put_str("; User pool physical address: ");
    put_int(p.phy_addr_start);
    put_char('\n');
}

void mem_init(void) {
    put_str("Init memory start.\n");
    uint32_t total_memory = (*(uint32_t*) (0xb00));
    mem_pool_init(total_memory);
    put_str("Init memory done.\n");
}