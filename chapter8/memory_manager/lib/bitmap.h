#ifndef _LIB_KERNEL_BITMAP_H
#define _LIB_KERNEL_BITMAP_H

# include "global.h"

# define BITMAP_MASK 1

struct bitmap {
    uint32_t btmp_bytes_len;
    uint8_t* bits;
};

void bitmap_init(struct bitmap* btmap);

void bitmap_scan_test(struct bitmap* btmap, uint32_t bit_idx);

int bitmap_scan(struct bitmap* btmap, uint32_t cnt);

int bitmap_set(struct bitmap* btmap, uint32_t index, int8_t value);

#endif
