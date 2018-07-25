[bits 32]

; 对于CPU会自动压入错误码的中断类型，无需额外的操作
%define ERROR_CODE nop
; 如果CPU没有压入错误码，为了保持处理逻辑的一致性，我们需要手动压入一个0
%define ZERO push 0

extern put_str

section .data
intr_str db 'interrupt occur!', 0xa, 0
global intr_entry_table
intr_entry_table:

    ; 中断处理程序宏定义
    %macro VECTOR 2
    section .text
    intr%1entry:
        %2
        push intr_str
        call put_str
        add esp, 4

        mov al, 0x20
        out 0xa0, al
        out 0x20, al

        add esp, 4
        iret

.section data:
    dd intr%1entry

%endmacro

VECTOR 0x00, ZERO
VECTOR 0x01, ZERO
VECTOR 0x02, ZERO
VECTOR 0x03, ZERO
VECTOR 0x04, ZERO
VECTOR 0x05, ZERO
VECTOR 0x06, ZERO
VECTOR 0x07, ZERO
VECTOR 0x08, ZERO
VECTOR 0x09, ZERO
VECTOR 0x0a, ZERO
VECTOR 0x0b, ZERO
VECTOR 0x0c, ZERO
VECTOR 0x0d, ZERO
VECTOR 0x0e, ZERO
VECTOR 0x0f, ZERO
VECTOR 0x10, ZERO
VECTOR 0x11, ZERO
VECTOR 0x12, ZERO
VECTOR 0x13, ZERO
VECTOR 0x14, ZERO
VECTOR 0x15, ZERO
VECTOR 0x16, ZERO
VECTOR 0x17, ZERO
VECTOR 0x18, ZERO
VECTOR 0x19, ZERO
VECTOR 0x1a, ZERO
VECTOR 0x1b, ZERO
VECTOR 0x1c, ZERO
VECTOR 0x1d, ZERO
VECTOR 0x1e, ERROR_CODE
VECTOR 0x1f, ZERO
VECTOR 0x20, ZERO