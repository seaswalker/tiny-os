# include "list.h"
# include "interrupt.h"

/**
 * 链表初始化.
 */ 
void list_init(struct list* list) {
    list->head.prev = NULL;
    list->head.next = &list->tail;
    list->tail.next = NULL;
    list->tail.prev = &list->head;
}

/**
 * 在before前插入节点elem.
 */ 
void list_insert_before(struct list_elem* before, struct list_elem* elem) {
    enum intr_status old_status = intr_disable();

    before->prev->next = elem;
    elem->prev = before->prev;
    elem->next = before;

    before->prev = elem;

    intr_set_status(old_status);
}

/**
 * 将给定的元素添加至链表队首.
 */ 
void list_push(struct list* list, struct list_elem* elem) {
    list_insert_before(list->head.next, elem);
}

/**
 * 将给定的元素添加至链表队尾.
 */ 
void list_append(struct list* list, struct list_elem* elem) {
    list_insert_before(&list->tail, elem);
}

/**
 * 将指定的元素从其链表中脱离.
 */ 
void list_remove(struct list_elem* elem) {
    enum intr_status old_status = intr_disable();

    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;

    intr_set_status(old_status);
}

/**
 * 弹出第一个元素.
 */ 
struct list_elem* list_pop(struct list* list) {
    struct list_elem* result = list->head.next;

    list_remove(result);

    return result;
}

/**
 * 链表查找.
 */ 
int list_find(struct list* list, struct list_elem* elem) {
    struct list_elem* e = list->head.next;

    while (e != &list->tail) {
        if (e == elem) {
            return 1;
        }
        e = e->next;
    }
    
    return 0;
}

int list_empty(struct list* list) {
    return (list->head.next == &list->tail ? 1 : 0);
}

uint32_t list_length(struct list* list) {
    struct list_elem* e = list->head.next;
    uint32_t length = 0;

    while (e != &list->tail) {
        ++length;
        e = e->next;
    }
    
    return length;
}

/**
 * 遍历链表，将链表元素和参数arg传给函数func，如果返回1，那么返回当前元素.
 */ 
struct list_elem* list_traversal(struct list* list, function func, int arg) {
    if (list_empty(list)) {
        return NULL;
    }

    struct list_elem* e = list->head.next;
     while (e != &list->tail) {
        if (func(e, arg)) {
            return e;
        }
        e = e->next;
    }

    return NULL;
}