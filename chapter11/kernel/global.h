#ifndef _KERNEL_GLOBAL_H
#define _KERNEL_GLOBAL_H

# include "stdint.h"

# define RPL0 0
# define RPL1 1
# define RPL2 2
# define RPL3 3

# define TI_GDT 0
# define TI_LDT 1

# define SELECTOR_K_CODE ((1 << 3) + (TI_GDT << 2) + RPL0)
# define SELECTOR_K_DATA ((2 << 3) + (TI_GDT << 2) + RPL0)
# define SELECTOR_K_STACK SELECTOR_K_DATA
# define SELECTOR_K_GS ((3 << 3) + (TI_GDT << 2) + RPL0)

# define SELECTOR_U_CODE ((5 << 3) + (TI_GDT << 2) + RPL3)
# define SELECTOR_U_DATA ((6 << 3) + (TI_GDT << 2) + RPL3)
# define SELECTOR_U_STACK SELECTOR_U_DATA

/* IDT描述符属性 */
# define IDT_DESC_P 1
# define IDT_DESC_DPL0 0
# define IDT_DESC_DPL3 3
# define IDT_DESC_32_TYPE 0xE
# define IDT_DESC_16_TYPE 0x6

# define IDT_DESC_ATTR_DPL0 \
    ((IDT_DESC_P << 7) + (IDT_DESC_DPL0 << 5) + IDT_DESC_32_TYPE)

# define IDT_DESC_ATTR_DPL3 \
    ((IDT_DESC_P << 7) + (IDT_DESC_DPL3 << 5) + IDT_DESC_32_TYPE)

# define NULL 0

// GDT描述符属性
# define DESC_G_4K 1
# define DESC_D_32 1
# define DESC_L 0
# define DESC_AVL 0
# define DESC_P 1
# define DESC_DPL_0 0
# define DESC_DPL_1 1
# define DESC_DPL_2 2
# define DESC_DPL_3 3
# define DESC_S_CODE 1
# define DESC_S_DATA DESC_S_CODE
# define DESC_S_SYS 0
# define DESC_TYPE_CODE 8
# define DESC_TYPE_DATA 2
# define DESC_TYPE_TSS 9

# define GDT_ATTR_HIGH ((DESC_G_4K << 7) + (DESC_D_32 << 6) + (DESC_L << 5) + (DESC_AVL << 4))
# define GDT_CODE_ATTR_LOW_DPL3 ((DESC_P << 7) + (DESC_DPL_3 << 5) + (DESC_S_CODE << 4) + (DESC_TYPE_CODE))
# define GDT_DATA_ATTR_LOW_DPL3 ((DESC_P << 7) + (DESC_DPL_3 << 5) + (DESC_S_DATA << 4) + (DESC_TYPE_DATA))

// TSS描述符属性
# define TSS_DESC_D 0
# define TSS_ATTR_HIGH ((DESC_G_4K << 7) + (TSS_DESC_D << 6) + (DESC_L << 5) + (DESC_AVL << 4) + 0x0)
# define TSS_ATTR_LOW ((DESC_P << 7) + (DESC_DPL_0 << 5) + (DESC_S_SYS << 4) + (DESC_TYPE_TSS))
# define SELECTOR_TSS ((4 << 3) + (TI_GDT << 2) + RPL0)

/**
 * GDT描述符结构.
 */ 
struct gdt_desc {
    uint16_t limit_low_word;
    uint16_t base_low_word;
    uint8_t base_mid_byte;
    uint8_t attr_low_byte;
    uint8_t limit_high_attr_byte;
    uint8_t base_high_byte;
};

# define EFLAGS_MBS (1 << 1)
# define EFLAGS_IF_1 (1 << 9)
# define EFLAGS_IF_0 0
# define EFLAGS_IOPL_3 (3 << 12)
# define EFLAGS_IOPL_0 (0 << 12)
# define DIV_ROUND_UP(X, STEP) ((X + STEP - 1) / STEP)
# define PAGE_SIZE 4096

#endif