# include "stdint.h"
# include "global.h"
# include "string.h"
# include "kernel/print.h"
# include "thread/thread.h"

/**
 * TSS结构体.
 */ 
struct tss {
    uint32_t backlink;
    uint32_t* esp0;
    uint32_t ss0;
    uint32_t* esp1;
    uint32_t ss1;
    uint32_t* esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t (*eip) (void);
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;
    uint32_t trace;
    uint32_t io_base;
};

static struct tss tss;

/**
 * 将TSS中的esp0更新为给定线程的0级栈.
 */ 
void update_tss_esp(struct task_struct* pthread) {
    tss.esp0 = (uint32_t*) ((uint32_t) pthread + PAGE_SIZE);
}

/**
 * 构造GDT描述符.
 */ 
static struct gdt_desc make_gdt_desc(uint32_t* desc_addr, uint32_t limit, uint8_t attr_low, uint8_t attr_high) {
    uint32_t desc_base = (uint32_t) desc_addr;
    struct gdt_desc desc;
    desc.limit_low_word = limit & 0x0000ffff;
    desc.base_low_word = desc_base & 0x0000ffff;
    desc.base_mid_byte = ((desc_base & 0x00ff0000) >> 16);
    desc.attr_low_byte = (uint8_t) attr_low;
    desc.limit_high_attr_byte = (((limit & 0x000f0000) >> 16) + (uint8_t) (attr_high));
    desc.base_high_byte = desc_base >> 24;
    return desc;
}

/**
 * 在GDT中创建TSS并加载之.
 */ 
void tss_init() {
    put_str("Tss init start...\n");

    uint32_t tss_size = sizeof(tss);
    memset(&tss, 0, tss_size);

    tss.ss0 = SELECTOR_K_STACK;
    tss.io_base = tss_size;

    // tss安装到gdt的第四个位置
    *((struct gdt_desc*) 0xc0000920) = make_gdt_desc((uint32_t*) &tss, tss_size - 1, TSS_ATTR_LOW, TSS_ATTR_HIGH);

    // dpl为3的代码段
    *((struct gdt_desc*) 0xc0000928) = make_gdt_desc((uint32_t*) 0, 0xfffff, GDT_CODE_ATTR_LOW_DPL3, GDT_ATTR_HIGH);

    // dpl为3的数据段
    *((struct gdt_desc*) 0xc0000930) = make_gdt_desc((uint32_t*) 0, 0xfffff, GDT_DATA_ATTR_LOW_DPL3, GDT_ATTR_HIGH);

    uint64_t gdt_operand = ((8 * 7 - 1) | ((uint64_t) (uint32_t) 0xc0000900 << 16));

    asm volatile ("lgdt %0" : : "m" (gdt_operand));
    asm volatile ("ltr %w0" : : "r" (SELECTOR_TSS));
    
    put_str("Init tss done.\n");
}