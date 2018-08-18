[bits 32]
section .text
global switch_to
switch_to:
    ; 将中断处理函数的上下文保存到目标PCB的栈中
    ; 这里隐含的是下次调度返回的地址其实是switch_to的返回地址
    push esi
    push edi
    push ebx
    push ebp
    mov eax, [esp + 20]
    mov [eax], esp

    ; 跳到目标PCB执行
    mov eax, [esp + 24]
    mov esp, [eax]
    pop ebp
    pop ebx
    pop edi
    pop esi
    ret