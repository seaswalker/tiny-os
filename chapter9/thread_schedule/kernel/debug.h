# ifndef _KERNEL_DEBUG_H
# define _KERNEL_DEBUG_H

void panic_spin(char* filename, int line, const char* func, const char* condition);

/**
 * 当断言被触发时调用.
 * _FILE_: 内置宏,表示调用的文件名
 * _LINE_: 内置宏,被编译文件的行号
 * _func_: 内置宏: 被编译的函数名
 * _VA_ARGS_: 函数调用参数
 */ 
# define PANIC(...) panic_spin (__FILE__, __LINE__, __func__, __VA_ARGS__)

# ifdef NDEBUG
    # define ASSERT(CONDITION) ((void) 0)
# else
    # define ASSERT(CONDITION) \
    if (CONDITION) { \
    } else { \
        PANIC(#CONDITION); \
    }
# endif
# endif