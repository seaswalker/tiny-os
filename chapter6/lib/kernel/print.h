/**
 * 为print.asm提供方便引用的头文件定义.
 */
#ifndef _LIB_KERNEL_PRINT_H
#define _LIB_KERNEL_PRINT_H

# include "stdint.h"

void put_char(uint8_t char_asci);

/**
 *  字符串打印，必须以\0结尾.
 */ 
void put_str(char* message);

#endif