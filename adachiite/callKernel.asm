bits 64
global callKernel

section .text
callKernel:
    ;mov rdi, qword [adachiiteBootInfoPtr]
    mov rbx, 0xffffffff80000000
    jmp rbx
    mov rax, 0xdeadbeef
    jmp $ ; if kernel return

section .data
global adachiiteBootInfoPtr
adachiiteBootInfoPtr:
    dq 0