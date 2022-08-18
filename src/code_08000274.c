#include "types.h"
#include "attributes.h"
#include "unknown.h"
#include "agbio.h"

enum
{
    DPAD_REPEAT_DELAY = 20,
    DPAD_REPEAT_INTERVAL = 4,
};

struct Unk_03002410 COMMON(Unk_03002410) Unk_03002410 = { 0 };

void func_08000274(struct Unk_03002410 * unk)
{
    fu8 i, j;

    unk->unk_00 = 0;
    unk->keys_held = 0;
    unk->keys_new = 0;
    unk->unk_01 = 0;

    for (i = 0; i < NUM_TASK; i++)
    {
        for (j = 0; j < NUM_TASK_00; j++)
        {
            *((unk->tasks + i)->unk_00 + j) = 0;
        }
    }
}

void func_08000314(int arg_0)
{
    struct Task * var_04;
    struct Task * var_08;
    int var_0C;

    var_0C = Unk_03002410.unk_01;
    var_04 = Unk_03002410.tasks + var_0C;

    var_04->unk_00[0] = ENUM_TASK_00_1;
    var_04->unk_00[2] = arg_0;

    while (TRUE)
    {
        while (TRUE)
        {
            var_08 = Unk_03002410.tasks + Unk_03002410.unk_01;

            switch (var_08->unk_00[0])
            {
            case ENUM_TASK_00_3:
                var_08->unk_00[0] = ENUM_TASK_00_4;
                break;

            case ENUM_TASK_00_4:
                if (var_08 == var_04)
                    return;
                else
                    var_08->func(var_08);

                break;

            case ENUM_TASK_00_1:
                if (var_08->unk_00[2] != 0)
                {
                    var_08->unk_00[2]--;

                    if (var_08->unk_00[2] == 0)
                        var_08->unk_00[0] = ENUM_TASK_00_4;
                }

                // fallthrough

            case ENUM_TASK_00_0:
            case ENUM_TASK_00_2:
            default:
                break;
            }

            Unk_03002410.unk_01++;

            if (Unk_03002410.unk_01 > 4)
            {
                Unk_03002410.unk_01 = 0;
                break;
            }
        }

        func_080018D0();
        func_08000690(&Unk_03002410);
    }
}

void func_08000470(fu8 arg_0)
{
    (Unk_03002410.tasks + Unk_03002410.unk_01)->unk_00[2] = arg_0;
}

void func_080004B0(struct Unk_03002410 * unk)
{
    while (TRUE)
    {
        for (unk->unk_01 = 0; unk->unk_01 < NUM_TASK; unk->unk_01++)
        {
            struct Task * var_04 = unk->tasks + unk->unk_01;

            if (var_04->unk_00[0] == ENUM_TASK_00_3)
            {
                var_04->unk_00[0] = ENUM_TASK_00_4;
                continue;
            }

            if (var_04->unk_00[0] == ENUM_TASK_00_4)
            {
                unk->tasks[unk->unk_01].func(unk->tasks + unk->unk_01);
            }
        }

        func_080018D0();
        func_08000690(unk);
    }
}

struct Task * func_08000570(void (* func)(struct Task * self), struct Unk_03002410 * unk, fu8 slot)
{
    fu8 dummy;
    fu8 slot_real;

    if (slot != CONSTANT_UNK_03002410_SLOT_AUTO)
        slot_real = slot;
    else
        slot_real = func_08000640(unk);

    unk->tasks[slot_real].unk_00[0] = ENUM_TASK_00_3;
    unk->tasks[slot_real].unk_00[2] = 0;
    unk->tasks[slot_real].func = func;

    unk->unk_00++;

    return unk->tasks + slot_real;
}

int func_08000640(struct Unk_03002410 * unk)
{
    fu8 slot;

    for (slot = 0; slot < NUM_TASK; slot++)
    {
        if (unk->tasks[slot].unk_00[0] == ENUM_TASK_00_0)
            break;
    }

    return slot;
}

void func_08000690(struct Unk_03002410 * unk)
{
    fu16 keys = ~REG_KEYINPUT;

    unk->keys_new = keys & (keys ^ unk->keys_held);
    unk->keys_held = keys;

    if (keys & KEYS_DPAD)
    {
        if (unk->dpad_repeat_delay_counter < DPAD_REPEAT_DELAY)
        {
            unk->dpad_repeat_delay_counter++;
        }
        else
        {
            if (unk->dpad_repeat_interval_counter == 0)
            {
                unk->keys_new &= ~KEYS_DPAD;
                unk->keys_new |= keys & KEYS_DPAD;
            }

            unk->dpad_repeat_interval_counter++;

            if (unk->dpad_repeat_interval_counter > DPAD_REPEAT_INTERVAL)
                unk->dpad_repeat_interval_counter = 0;
        }
    }
    else
    {
        unk->dpad_repeat_delay_counter = 0;
        unk->dpad_repeat_interval_counter = 0;
    }
}

void func_080007B8(void)
{
}
