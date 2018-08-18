# ifndef _USER_TSS_H
# define _USER_TSS_H

void update_tss_esp(struct task_struct* pthread);
void tss_init();

# endif