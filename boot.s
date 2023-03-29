# boot process

loop:
    wfi
    j           loop

.section .text
.globl _start
_start:
    jal         loop

.end
