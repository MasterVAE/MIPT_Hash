DEFAULT REL

section .text
global Hash_ASM

Hash_ASM:
    mov rax, 5381d
    
.loop:
    movzx rcx, BYTE [rsi]
    test rcx, rcx
    je .exit

    inc rsi
    xor rax, rcx
    rol rax, 1

    jmp .loop

.exit:
    xor rdx, rdx
    div rdi
    mov rax, rdx

    ret