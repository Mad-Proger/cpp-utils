.intel_syntax noprefix

.text
.global SwitchContext
.global CreateContext

SwitchContext:
    lea rax, [rip + SwitchImpl]
    jmp SaveRegisters

SwitchImpl:
    mov [rsi], rsp
    mov rsp, rdi
    jmp LoadRegisters

CreateContext:
    lea rax, [rip + ExecuteTrampoline]
    jmp SaveRegisters

ExecuteTrampoline:
    # swap(rdi, rsp)
    xor rdi, rsp
    xor rsp, rdi
    xor rdi, rsp

    # align new stack for 16 bytes
    and rsp, 0xFFFFFFFFFFFFFFF0
    call rdx
    mov rsp, rax
    jmp LoadRegisters

SaveRegisters:
    push rbx
    push rbp
    push r12
    push r13
    push r14
    push r15
    jmp rax

LoadRegisters:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    pop rbx
    ret
