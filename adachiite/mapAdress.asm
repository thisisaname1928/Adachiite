bits 64
section .text
global getCr3
global invlpgM

; use system v abi
getCr3:
    mov rax, cr3
    ret

invlpgM:
    mov rax, [PML4Address]
    mov cr3, rax
    ret

section .data
global PML4Address
; idk why rdi != args 1 so Im use this instead
PML4Address:
    dq 0

