#include "agbprint.h"
#include "agbio.h"

#include <stdarg.h>
#include <stdio.h>

struct AgbPrintSt
{
    /* 00 */ u16 request;
    /* 02 */ u16 bank;
    /* 04 */ u16 get;
    /* 06 */ u16 put;
};

typedef void (* AgbPrintFlushFuncPtr)(void);

#define AGB_PRINT_FLUSH   ((AgbPrintFlushFuncPtr) 0x09FE209D)
#define AGB_PRINT_STRUCT  ((struct AgbPrintSt *)  0x09FE20F8)
#define AGB_PRINT_PROTECT ((u16 *)                0x09FE2FFE)

enum { AGB_PRINT_WAITCNT_CONFIG = WAITCNT_PHI_OUT_16MHZ | WAITCNT_ROM_S_2 | WAITCNT_ROM_N_4 };

void AgbPrintFlush1Block(void);

void AgbPrintInit(void)
{
    struct AgbPrintSt * st = AGB_PRINT_STRUCT;
    u16 volatile * p_waitcnt = &REG_WAITCNT;
    u16 * protect = AGB_PRINT_PROTECT;
    u16 waitcnt = *p_waitcnt;
    *p_waitcnt = AGB_PRINT_WAITCNT_CONFIG;
    *protect = 0x20;
    st->request = st->get = st->put = 0;
    st->bank = 0xFD;
    *protect = 0;
    *p_waitcnt = waitcnt;
}

static void AgbPutcInternal(char chr)
{
    struct AgbPrintSt volatile * st = AGB_PRINT_STRUCT;
    u16 * print_buf = (u16 *)(0x08000000 + (st->bank << 16));
    u16 * protect = AGB_PRINT_PROTECT;
    u16 data = print_buf[st->put / 2];
    *protect = 0x20;
    data = (st->put & 1) ? (data & 0xFF) | (chr << 8) : (data & 0xFF00) | chr;
    print_buf[st->put / 2] = data;
    st->put++;
    *protect = 0;
}

void AgbPutc(char chr)
{
    u16 volatile * p_waitcnt = &REG_WAITCNT;
    u16 waitcnt = *p_waitcnt;
    struct AgbPrintSt * st;
    *p_waitcnt = AGB_PRINT_WAITCNT_CONFIG;
    AgbPutcInternal(chr);
    *p_waitcnt = waitcnt;

    st = AGB_PRINT_STRUCT;

    if (st->put == ((st->get - 1) & 0xFFFF))
        AgbPrintFlush1Block();
}

void AgbPrint(char const * str)
{
    struct AgbPrintSt volatile * st = AGB_PRINT_STRUCT;
    u16 volatile * p_waitcnt = &REG_WAITCNT;
    u16 waitcnt = *p_waitcnt;
    *p_waitcnt = AGB_PRINT_WAITCNT_CONFIG;

    while (*str)
    {
        AgbPutc(*str);
        str++;
    }

    *p_waitcnt = waitcnt;
}

void AgbPrintf(char const * fmt, ...)
{
    char buf[0x100];
    va_list args;

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    AgbPrint(buf);
}

static void AgbPrintTransferDataInternal(int all_data)
{
    AgbPrintFlushFuncPtr flush_func;
    u16 volatile * p_ime;
    u16 ime;
    u16 volatile * p_waitcnt;
    u16 waitcnt;
    u16 * protect;
    struct AgbPrintSt volatile * st;

    protect = AGB_PRINT_PROTECT;
    st = AGB_PRINT_STRUCT;
    flush_func = AGB_PRINT_FLUSH;
    p_ime = &REG_IME;
    ime = *p_ime;
    p_waitcnt = &REG_WAITCNT;
    waitcnt = *p_waitcnt;
    *p_ime = ime & ~1;
    *p_waitcnt = AGB_PRINT_WAITCNT_CONFIG;

    if (all_data)
    {
        while (st->put != st->get)
        {
            *protect = 0x20;
            flush_func();
            *protect = 0;
        }
    }
    else if (st->put != st->get)
    {
        *protect = 0x20;
        flush_func();
        *protect = 0;
    }

    *p_waitcnt = waitcnt;
    *p_ime = ime;
}

void AgbPrintFlush1Block(void)
{
    AgbPrintTransferDataInternal(FALSE);
}

void AgbPrintFlush(void)
{
    AgbPrintTransferDataInternal(TRUE);
}

void AgbAssert(char const * file, int line, char const * expr, int is_fatal)
{
    if (is_fatal)
    {
        AgbPrintf("ASSERTION FAILED  FILE=[%s] LINE=[%d]  EXP=[%s] \n", file, line, expr);
        AgbPrintFlush();
        asm(".2byte 0xEFFF");
    }
    else
    {
        AgbPrintf("WARING FILE=[%s] LINE=[%d]  EXP=[%s] \n", file, line, expr);
    }
}
