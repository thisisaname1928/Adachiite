bits 64
section .text
global getCr3
global invlpgM
global PML4Address

; use system v abi
getCr3:
    mov rax, cr3
    ret

invlpgM:
    mov rax, rcx
    mov cr3, rax
    ret

section .data
; idk why rdi != args 1 so Im use this instead
PML4Address:
    dq 0

