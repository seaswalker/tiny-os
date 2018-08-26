# ifndef _USER_PROCESS_H

# include "thread/thread.h"

# define _USER_PROCESS_H

# define USER_STACK3_VADDR (0xc0000000 - 0x1000)
# define USER_VADDR_START 0x8048000
# define default_prio 31

void start_process(void* filename);
void page_dir_activate(struct task_struct* pthread);
void process_activate(struct task_struct* pthread);
void create_user_vaddr_bitmap(struct task_struct* user_process);
void process_execute(void* filename, char* name);

# endif