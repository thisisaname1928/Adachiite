bits 64
global callKernel

section .text
callKernel:
    mov rdi, qword [adachiiteBootInfoPtr]
    mov rbx, qword [adachiitekernelEntry]
    call rbx
    mov rax, 0xdeadbeef
    jmp $ ; if kernel return

section .data
global adachiiteBootInfoPtr
global adachiitekernelEntry
adachiiteBootInfoPtr:
    dq 0
adachiitekernelEntry:
    dq 0