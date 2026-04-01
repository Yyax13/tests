BITS 64
global _start

_start:
    mov rax, 0x3c               ; exit
    xor rdi, rdi                ; rdi = 0

    syscall                     ; Syscall exit
