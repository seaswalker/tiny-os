# ifndef _LIB_KERNEL_LIST_H
# define _LIB_KERNEL_LIST_H

# include "global.h"

/**
 * 获取结构体内成员在结构体的偏移.
 */ 
# define offset(struct_type, member) (int) ($((struct_type*)0)->member);

# define ele2entry(struct_type, struct_member_name, elem_ptr) \
    (struct_type*) ((int) elem_ptr - offset(struct_type, struct_member_name));

/**
 * 链表节点.
 */ 
struct list_elem {
    struct list_elem* prev;
    struct list_elem* next;
};

/**
 * 链表结构.
 */ 
struct list {
    struct list_elem head;
    struct list_elem tail;
};

/**
 * 用于链表遍历的回调函数.
 */ 
typedef int (function) (struct list_elem*, int arg);

# endif