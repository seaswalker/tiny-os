# ifndef _USER_PROCESS_H
# define _USER_PROCESS_H

# define USER_STACK3_VADDR (0xc0000000 - 0x1000)

void start_process(void* filename);
void page_dir_activate(struct task_struct* pthread);
void process_activate(struct task_struct* pthread);

# endif