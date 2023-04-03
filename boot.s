# Boot process

.equ            STACK_SIZE, 1024
.equ            CPU_NUM, 8

.section .text
.globl _start

_start:
    csrr        t0, mhartid
    bnez        t0, park

    slli        t0, t0, 10
    la          sp, stacks + STACK_SIZE
    add         sp, sp, t0

    j           start_kernel

stacks:
    .space      STACK_SIZE * CPU_NUM 

park:
    wfi
    j           park

.end
