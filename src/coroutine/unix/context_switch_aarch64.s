.text
.global SwitchContext
.global SetupContext

SwitchContext:
    adr x4, EnterStack
    b LeaveStack

SetupContext:
    adr x4, ExecuteTrampoline
    b LeaveStack

ExecuteTrampoline:
    # align new stack for 16 bytes
    mov x0, sp
    bic x0, x0, 0xF
    mov sp, x0
    mov x0, x2
    blr x3

LeaveStack:
    stp x19, x20, [sp, -16]!
    stp x21, x22, [sp, -16]!
    stp x23, x24, [sp, -16]!
    stp x25, x26, [sp, -16]!
    stp x27, x28, [sp, -16]!
    stp x29, x30, [sp, -16]!

    stp  q8,  q9, [sp, -32]!
    stp q10, q11, [sp, -32]!
    stp q12, q13, [sp, -32]!
    stp q14, q15, [sp, -32]!

    mov x5, sp
    str x5, [x1]
    mov sp, x0

    br x4

EnterStack:
    ldp q14, q15, [sp, 32]!
    ldp q12, q13, [sp, 32]!
    ldp q10, q11, [sp, 32]!
    ldp  q8,  q9, [sp, 32]!

    ldp x29, x30, [sp, 16]!
    ldp x27, x28, [sp, 16]!
    ldp x25, x26, [sp, 16]!
    ldp x23, x24, [sp, 16]!
    ldp x21, x22, [sp, 16]!
    ldp x19, x20, [sp, 16]!

    ret
