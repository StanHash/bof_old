    .include "asm_macros.inc"
    .include "asm_agbio.inc"

    .text

    .syntax unified
    .thumb

    function BgAffineSet
BgAffineSet:
    svc  0x0E
    bx   lr

    endfunction BgAffineSet

    function CpuFastSet
CpuFastSet:
    svc  0x0C
    bx   lr

    endfunction CpuFastSet

    function CpuSet
CpuSet:
    svc  0x0B
    bx   lr

    endfunction CpuSet

    function DivArm
DivArm:
    svc  0x07
    bx   lr

    endfunction DivArm

    function DivArmRem
DivArmRem:
    svc  0x07
    adds r0, r1, 0
    bx   lr

    endfunction DivArmRem

    function VBlankIntrWait
VBlankIntrWait:
    movs r2, 0
    svc  0x05
    bx   lr

    endfunction VBlankIntrWait

    function SoftReset
SoftReset:
    ldr  r3, =REG_IME
    movs r2, 0
    strb r2, [r3]
    ldr  r3, =0x03007FFA
    movs r2, 0
    strb r2, [r3]
    subs r3, 0x03007FFA - 0x03007F00
    mov  sp, r3
    svc  0x01
    svc  0x00

    .align 2, 0
    .pool

    endfunction SoftReset
