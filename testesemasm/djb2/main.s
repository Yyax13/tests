BITS 64

section .data
    STRS db 'howo', 0
    STRL equ $ - STRS
    HASH dq 5381

section .text
    global _start                                       ; Set the entrypoint as the function _start

_start:                                                 ; main function
    xor rax, rax                                        ; rax = 0
    mov rax, [HASH]                                     ; rax = &HASH
    
    xor rsi, rsi                                        ; rsi = 0
    mov rsi, STRS                                       ; rsi = STRS

    jmp djb2_loop                                       ; goto djb2_loop

djb2_loop:
                                                        ; bl is the least significant byte of rbx
    mov bl, [rsi]                                       ; bl = &rsi
    cmp bl, 0x00                                        ; if (bl)
    je end                                              ;   goto end;

    mov rcx, rax                                        ; Copy rax to rcx because we need actual value after the fisrt change
    shl rax, 5                                          ; rax << 5
    add rax, rcx                                        ; rax += rcx
    add rax, rbx                                        ; rax += rbx
                                                        ; rbx is the current byte of STRS set in mov bl, [rsi] as bl is rbx lower byte
    
    inc rsi                                             ; rsi = rsi[i + 1]
    jmp djb2_loop                                       ; goto next iteration

end:
    jmp exit_0

exit_0:                                                 ; Exit with status code 0
    xor rdi, rdi                                        ; rdi = 0
    xor rax, rax                                        ; rax = 0
    mov rax, 0x3c                                       ; rax = 60
    
    syscall                                             ; call syscall exit
