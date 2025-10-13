.intel_syntax noprefix

.text
.global SwitchContext
.global SetupContext

SwitchContext:
    lea rax, [rip + EnterStack]
    jmp LeaveStack

SetupContext:
    lea rax, [rip + ExecuteTrampoline]
    jmp LeaveStack

ExecuteTrampoline:
    # align new stack for 16 bytes
    and rsp, 0xFFFFFFFFFFFFFFF0
    mov rdi, rdx
    call rcx

LeaveStack:
    push rbx
    push rbp
    push r12
    push r13
    push r14
    push r15

    mov [rsi], rsp
    mov rsp, rdi

    jmp rax

EnterStack:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    pop rbx

    ret
