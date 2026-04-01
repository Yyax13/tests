BITS 64

section .data
    HASH dq 5381

section .text
    global hash                                         ; Extern the function hash

hash:
    xor r8, r8                                          ; r8 = 0
    xor rax, rax                                        ; rax = 0
    mov rax, [rel HASH]                                 ; rax = HASH (rax = 5381)
    
    call djb2_loop                                      ; djb2_loop()
    ret    

djb2_loop:
                                                        ; bl is the least significant byte of rbx
    mov bl, [rdi + r8]                                  ; bl = &rdi + index
    cmp r8, rsi                                         ; if (r8 >= rsi)
    jge djb2_end                                        ;   goto djb2_end;

    mov rcx, rax                                        ; Copy rax to rcx because we need actual value after the fisrt change
    shl rax, 5                                          ; rax <<= 5
    add rax, rcx                                        ; rax += rcx
    add rax, rbx                                        ; rax += rbx
                                                        ; rbx is the current byte of rdi set in mov bl, [rdi] as bl is rbx lower byte
    
    inc r8                                              ; rsi = rsi[i + 1]
    jmp djb2_loop                                       ; goto next iteration

djb2_end:
    mov rdx, rsi                                        ; rdx = rsi
    ret                                                 ; return