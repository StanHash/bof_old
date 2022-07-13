#pragma once

#include "types.h"

enum
{
    RESET_EWRAM      = 0x01,
    RESET_IWRAM      = 0x02,
    RESET_PALETTE    = 0x04,
    RESET_VRAM       = 0x08,
    RESET_OAM        = 0x10,
    RESET_SIO_REGS   = 0x20,
    RESET_SOUND_REGS = 0x40,
    RESET_REGS       = 0x80,
    RESET_ALL        = 0xFF,
};

enum
{
    CPU_SET_SRC_FIXED = 0x01000000,
    CPU_SET_16BIT     = 0x00000000,
    CPU_SET_32BIT     = 0x04000000,
};

struct BgAffineSrc
{
    /* 00 */ i32 x_tex;
    /* 04 */ i32 y_tex;
    /* 08 */ i16 x_scr;
    /* 0A */ i16 y_scr;
    /* 0C */ i16 sx;
    /* 0E */ i16 sy;
    /* 10 */ u16 alpha;
};

struct BgAffineDst
{
    /* 00 */ i16 pa;
    /* 02 */ i16 pb;
    /* 04 */ i16 pc;
    /* 06 */ i16 pd;
    /* 08 */ i32 dx;
    /* 0C */ i32 dy;
};

void BgAffineSet(struct BgAffineSrc const * src, struct BgAffineDst * dst, int count);
void CpuFastSet(void const * src, void * dst, u32 control);
void CpuSet(void const * src, void * dst, u32 control);
int DivArm(int divisor, int dividend);
int DivArmRem(int divisor, int dividend);
void VBlankIntrWait(void);
void SoftReset(u32 reset_flags);

#define CPU_FILL(value, dest, size, bit)                                          \
{                                                                                 \
    u##bit volatile tmp = (u##bit volatile)(value);                               \
    CpuSet((void *) &tmp,                                                         \
           (dest),                                                                \
           CPU_SET_##bit##BIT | CPU_SET_SRC_FIXED | ((size)/(bit/8) & 0x1FFFFF)); \
}

#define CpuFill16(value, dest, size) CPU_FILL(value, dest, size, 16)
#define CpuFill32(value, dest, size) CPU_FILL(value, dest, size, 32)

#define CPU_COPY(src, dest, size, bit) CpuSet(src, dest, CPU_SET_##bit##BIT | ((size)/(bit/8) & 0x1FFFFF))

#define CpuCopy16(src, dest, size) CPU_COPY(src, dest, size, 16)
#define CpuCopy32(src, dest, size) CPU_COPY(src, dest, size, 32)

#define CpuFastFill(value, dest, size)                          \
{                                                               \
    u32 volatile tmp = (u32 volatile)(value);                   \
    CpuFastSet((void *) &tmp,                                   \
               (dest),                                          \
               CPU_SET_SRC_FIXED | ((size)/(32/8) & 0x1FFFFF)); \
}

#define CpuFastFill16(value, dest, size) CpuFastFill(((value) << 16) | (value), (dest), (size))

#define CpuFastCopy(src, dest, size) CpuFastSet(src, dest, ((size)/(32/8) & 0x1FFFFF))
