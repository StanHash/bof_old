#include "agbsram.h"
#include "agbio.h"

char const AgbSramVersion[] = "SRAM_V111";

static void ReadSramFunc(u8 const * src, u8 * dst, u32 size)
{
    while (--size != -1)
        *dst++ = *src++;
}

void ReadSram(u8 const * src, u8 * dst, u32 size)
{
    u16 buf[0x40];

    u16 const * func_src;
    u16 * func_dst;
    u16 func_len;

    REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_8;

    func_src = (void *)((uptr) ReadSramFunc);
    func_src = (void *)((uptr) func_src ^ 1);
    func_dst = buf;
    func_len = ((uptr) ReadSram - (uptr) ReadSramFunc) / 2;

    while (func_len != 0)
    {
        *func_dst++ = *func_src++;
        --func_len;
    }

    ((void (*)(u8 const *, u8 *, u32)) buf + 1)(src, dst, size);
}

void WriteSram(u8 const * src, u8 * dst, u32 size)
{
    REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_8;

    while (--size != -1)
        *dst++ = *src++;
}

static u32 VerifySramFunc(u8 const * src, u8 * dst, u32 size)
{
    while (--size != -1)
    {
        if (*dst++ != *src++)
            return (u32)(dst - 1);
    }

    return 0;
}

u32 VerifySram(u8 const * src, u8 * dst, u32 size)
{
    u16 buf[0x60];

    u16 const * func_src;
    u16 * func_dst;
    u16 func_len;

    REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_8;

    func_src = (void *)((uptr) VerifySramFunc);
    func_src = (void *)((uptr) func_src ^ 1);
    func_dst = buf;
    func_len = ((uptr) VerifySram - (uptr) VerifySramFunc) / 2;

    while (func_len != 0)
    {
        *func_dst++ = *func_src++;
        --func_len;
    }

    return ((u32 (*)(u8 const *, u8 *, u32)) buf + 1)(src, dst, size);
}
