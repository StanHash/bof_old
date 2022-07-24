#pragma once

#include "types.h"

// ========
// = crt0 =
// ========

extern void IntrMain(void);

// =================
// = code_08000274 =
// =================

enum
{
    NUM_UNK_03002410_08_00 = 3,
    NUM_UNK_03002410_08 = 5,
};

enum
{
    ENUM_UNK_03002410_08_00_0,
    ENUM_UNK_03002410_08_00_1,
    ENUM_UNK_03002410_08_00_2,
    ENUM_UNK_03002410_08_00_3,
    ENUM_UNK_03002410_08_00_4,
};

struct Task
{
    /* 00 */ u8 unk_00[NUM_UNK_03002410_08_00];
    /* 03 */ i8 unk_03;
    /* 04 */ u32 unk_04;
    /* 08 */ void (* func)(struct Task * self);
};

enum
{
    CONSTANT_UNK_03002410_SLOT_AUTO = 0xFF,
};

struct Unk_03002410
{
    /* 00 */ u8 unk_00;
    /* 01 */ u8 unk_01;
    /* 02 */ u8 dpad_repeat_delay_counter;
    /* 03 */ u8 dpad_repeat_interval_counter;
    /* 04 */ u16 keys_held;
    /* 06 */ u16 keys_new;
    /* 08 */ struct Task tasks[NUM_UNK_03002410_08];
};

extern void func_08000274(struct Unk_03002410 * unk);
extern void func_08000314(int arg_0);
extern void func_08000470(fu8 arg_0);
extern void func_080004B0(struct Unk_03002410 * unk);
extern struct Task * func_08000570(void (* func)(struct Task * self), struct Unk_03002410 * unk, fu8 slot);
extern int func_08000640(struct Unk_03002410 * unk);
extern void func_08000690(struct Unk_03002410 * unk);
extern void func_080007B8(void);

extern struct Unk_03002410 Unk_03002410;

// =================
// = code_080007C4 =
// =================

typedef void (* IntrMainPtr)(void);
typedef void (* IntrFuncPtr)(void);
typedef void (* MainFuncPtr)(void);

extern void IntrInit(void);

extern IntrFuncPtr const IntrTableTemplate[];

extern IntrFuncPtr IntrTable[];
extern MainFuncPtr Unk_0300249C;

// ========
// = code =
// ========

extern void OnVBlank_08000D68(void);
extern void func_080018D0(void);
extern void func_0800198C(struct Task * self);
extern void func_08002500(struct Task * self);

// ==========
// = rodata =
// ==========

// ========
// = data =
// ========

extern MainFuncPtr const Unk_0817B400[2];
