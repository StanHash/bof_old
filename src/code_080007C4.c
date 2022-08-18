#include "types.h"
#include "macros.h"
#include "attributes.h"
#include "agbio.h"
#include "agbsyscalls.h"
#include "m4a.h"

#include "unknown.h"

// TODO: rename those properly and move them elsewhere
#define INTR_MAIN_PTR (*(IntrMainPtr *) 0x03007FFC)
#define INTR_CHECK (*(u16 *) 0x03007FF8)

static u32 IntrMainRam[0x200];

IntrFuncPtr const IntrTableTemplate[] =
{
    [IRQ_INDEX_VBLANK]    = OnVBlank_08000D68,
    [IRQ_INDEX_HBLANK]    = OnHBlank_08000F74,
    [IRQ_INDEX_VCOUNT]    = IntrDummy,
    [IRQ_INDEX_TIMER_0]   = IntrDummy,
    [IRQ_INDEX_TIMER_1]   = IntrDummy,
    [IRQ_INDEX_TIMER_2]   = IntrDummy,
    [IRQ_INDEX_TIMER_3]   = func_08117710,
    [IRQ_INDEX_SERIAL]    = func_08117710,
    [IRQ_INDEX_DMA_0]     = IntrDummy,
    [IRQ_INDEX_DMA_1]     = IntrDummy,
    [IRQ_INDEX_DMA_2]     = IntrDummy,
    [IRQ_INDEX_DMA_3]     = IntrDummy,
    [IRQ_INDEX_KEYPAD]    = IntrDummy,
    [IRQ_INDEX_CARTRIDGE] = IntrDummy,
    [IRQ_INDEX_UNK]       = IntrDummy,
};

void IntrInit(void)
{
    fu8 i;

    for (i = 0; i < IRQ_INDEX_MAX; i++)
        IntrTable[i] = IntrTableTemplate[i];

    DmaCopy16(3, IntrMain, IntrMainRam, sizeof(IntrMainRam));
    INTR_MAIN_PTR = (IntrMainPtr) IntrMainRam;

    REG_IME = TRUE;
    REG_IE = IRQ_VBLANK | IRQ_CARTRIDGE;
    REG_DISPSTAT = LCD_VBLANK_IRQ_ENABLE;

    MainFunc = Unk_0817B400[0];
    IntrTable[IRQ_INDEX_VBLANK] = OnVBlank_08000D68;
}

void SyncDisp(void)
{
    Unk_030024A8 = 0;

    if (Unk_03004ED8 & FLAG_UNK_03004ED8_0)
    {
        func_08001228();
        func_0806AA04(Unk_03002594);
        REG_DISPCNT = DispCnt;
        REG_BLDVAL = BlendVal;
        Unk_030023AC = Unk_03004140;
        func_08001050(Unk_02020FC0 + 1);

        if (Unk_030023C4 & FLAG_UNK_030023C4_ANYTM)
        {
            func_08000E24();
        }
        else
        {
            func_08047848(Unk_03004EC0);
        }

        func_080682C0();
        func_08068300();
        func_08072E3C();
    }

    REG_BG0CNT = Bg0Cnt;
    REG_BG1CNT = Bg1Cnt;
    REG_BG2CNT = Bg2Cnt;
    REG_BG3CNT = Bg3Cnt;

    if (Unk_030023C4 & FLAG_UNK_030023C4_BIT_20)
    {
        REG_BG0HOFS = Bg0HOffset;
        REG_BG0VOFS = Bg0VOffset;
        REG_BG1HOFS = Bg1HOffset + BgExtraHOffset;
        REG_BG1VOFS = Bg1VOffset + BgExtraVOffset;
        REG_BG2HOFS = Bg2HOffset + BgExtraHOffset;
        REG_BG2VOFS = Bg2VOffset + BgExtraVOffset;
        REG_BG3HOFS = Bg3HOffset;
        REG_BG3VOFS = Bg3VOffset;
        REG_BG2X = Bg2X_0300238C;
        REG_BG2Y = Bg2Y_03002388;
    }
    else
    {
        REG_BG0HOFS = Bg0HOffset + Unk_030023C8 + BgExtraHOffset;
        REG_BG0VOFS = Bg0VOffset + Unk_0300239C + BgExtraVOffset;
        REG_BG1HOFS = Bg1HOffset + Unk_030023C8 + BgExtraHOffset;
        REG_BG1VOFS = Bg1VOffset + Unk_0300239C + BgExtraVOffset;
        REG_BG2HOFS = Bg2HOffset + Unk_030023C8 + BgExtraHOffset;
        REG_BG2VOFS = Bg2VOffset + Unk_0300239C + BgExtraVOffset;
        REG_BG3HOFS = Bg3HOffset + Unk_030023C8 + BgExtraHOffset;
        REG_BG3VOFS = Bg3VOffset + Unk_0300239C + BgExtraVOffset;
    }

    if (Unk_03004E14 != 0)
    {
        REG_BG2X = Unk_03005350.x + (Unk_030023C8 << 8);
        REG_BG2Y = Unk_03005350.y + (Unk_0300239C << 8);
        REG_BG2PA = Unk_03005350.pa;
        REG_BG2PD = Unk_03005350.pd;
        REG_BG2PB = Unk_03005350.pb;
        REG_BG2PC = Unk_03005350.pc;
    }

    REG_BLDCNT = BlendCnt;
    REG_BLDALPHA = BlendAlpha;
    REG_MOSAIC = Mosaic;
    REG_WININ = WinIn;
    REG_WINOUT = WinOut;

    if (Unk_030023C4 & FLAG_UNK_030023C4_WINDIM)
    {
        REG_WIN0H = Win0H;
        REG_WIN0V = Win0V;
        REG_WIN1H = Win1H;
        REG_WIN1V = Win1V;
    }

    if (Unk_0202D2C0 != 0)
    {
        int i;

        for (i = 0; i < Unk_0202D2C0; i++)
        {
            // TODO: chr size constant?
            DmaCopy16(3, Unk_02019E00[i], Unk_0200B820[i], 0x20);
        }

        if (Unk_03004E1C == 0)
        {
            // TODO: not hardwire those offsets
            DmaCopy16(3, MEM_PALETTE + 0x20, MEM_PALETTE + 0x320, 0xE0);
        }
    }

    Unk_0202D2C0 = 0;
    Unk_03005350.unk_5B++;
}

void OnVBlank_08000D68(void)
{
    INTR_CHECK |= IRQ_VBLANK;

    func_08001188();

    Unk_030024A8 = 0;

    m4aSoundVSync();
    m4aSoundMain();

    Unk_03005350.unk_5B++;
}

void OnVBlank_08000DC8(void)
{
    INTR_CHECK |= IRQ_VBLANK;

    func_08001188();

    Unk_030024A8 = 0;

    m4aSoundVSync();

    Unk_03005350.unk_5B++;
}

void func_08000E24(void)
{
    if (Unk_030023C4 & FLAG_UNK_030023C4_BG0TM)
    {
        DmaCopy16(3, Unk_02019FC0, MEM_VRAM + 0x0000, sizeof(Unk_02019FC0));
        Unk_030023C4 &= ~FLAG_UNK_030023C4_BG0TM;
    }

    if (Unk_030023C4 & FLAG_UNK_030023C4_BG1TM)
    {
        DmaCopy16(3, Unk_0201A7C0, MEM_VRAM + 0x0800, sizeof(Unk_0201A7C0));
        Unk_030023C4 &= ~FLAG_UNK_030023C4_BG1TM;
    }

    if (Unk_030023C4 & FLAG_UNK_030023C4_BG2TM)
    {
        DmaCopy16(3, Unk_0201AFC0, MEM_VRAM + 0x1000, sizeof(Unk_0201AFC0));
        Unk_030023C4 &= ~FLAG_UNK_030023C4_BG2TM;
    }

    if (Unk_030023C4 & FLAG_UNK_030023C4_BG3TM)
    {
        DmaCopy16(3, Unk_0201B7C0, MEM_VRAM + 0x1800, sizeof(Unk_0201B7C0));
        Unk_030023C4 &= ~FLAG_UNK_030023C4_BG3TM;
    }
}

void OnHBlank_08000F74(void)
{
    if (REG_VCOUNT >= LCD_HEIGHT || (Unk_030023C4 & FLAG_UNK_030023C4_INHDMA))
        return;

    Unk_030023C4 |= FLAG_UNK_030023C4_INHDMA;

    if (Unk_03005C44.unk_00 != 0)
    {
        DmaCopy16(3, Unk_02011DA0 + Unk_030024A8, Unk_03005C3C, Unk_03005C44.unk_00);
        Unk_030024A8 += Unk_03005C44.unk_00;
    }

    Unk_030023C4 &= ~FLAG_UNK_030023C4_INHDMA;

    INTR_CHECK = IRQ_HBLANK;
}

void IntrDummy(void)
{
}

void func_08001050(void const * unused_0)
{
    // TODO: constants!

    Unk_03005178 = 0;

    if (Unk_030023C4 & FLAG_UNK_030023C4_BIT_0)
    {
        DmaCopy32(3, Unk_030054F8, MEM_PALETTE + (Unk_03004EBC << 5), Unk_03005C50);

        if (Unk_03005C50 != MEM_PALETTE_SIZE)
        {
            DmaCopy32(3, Unk_030054F8 + 8 * 0x10, MEM_PALETTE + 0x10 * 0x20, 0x100);
        }

        Unk_030023C4 &= ~FLAG_UNK_030023C4_BIT_0;
    }
}

void func_0800111C(void * dst, int length)
{
    REG_DMA0SRC = Unk_0200BE80;
    REG_DMA0DST = dst;
    REG_DMA0CNT_L = length >> 1;
    REG_DMA0CNT_H = (DMA_START_VBLANK | DMA_REPEAT | DMA_SRC_INCREMENT | DMA_DST_RELOAD) >> 16;

    Unk_03002390 = dst;
    Unk_03002378 = length >> 1;
    Unk_03002380 = Unk_0200BE80;
}

void func_08001188(void)
{
    REG_DMA0CNT_H &= ~DMA_ENABLE >> 16;

    if (Unk_03002378 != 0)
    {
        if (Unk_03002380 == Unk_0200BE80)
        {
            REG_DMA0SRC = Unk_0200BE80;
            Unk_03002380 = Unk_020213C0;
        }
        else
        {
            REG_DMA0SRC = Unk_020213C0;
            Unk_03002380 = Unk_0200BE80;
        }

        REG_DMA0DST = Unk_03002390;
        REG_DMA0CNT_L = Unk_03002378;
        REG_DMA0CNT_H = (DMA_ENABLE | DMA_START_VBLANK | DMA_REPEAT | DMA_SRC_INCREMENT | DMA_DST_RELOAD) >> 16;
    }
}

void func_08001228(void)
{
    if (Unk_030023CC != 0)
        return;

    if (Unk_030055D0.unk_2A != 0x20 && Unk_030055D0.unk_2A != 0x17 && Unk_030055D0.unk_2A != 0x1D)
    {
        Unk_03005C44.unk_00 = 0;
    }

    if (Unk_03005C44.unk_01 != 0)
    {
        DmaCopy16(3, Unk_020127A0, Unk_02011DA0, sizeof(Unk_020127A0));
        REG_IE = IRQ_VBLANK | IRQ_HBLANK | IRQ_CARTRIDGE;
        REG_DISPSTAT = LCD_VBLANK_IRQ_ENABLE | LCD_HBLANK_IRQ_ENABLE;

        Unk_03005C44.unk_00 = Unk_03005C44.unk_01;
        Unk_03005C44.unk_01 = 0;
    }
}

u16 const * func_080012FC(u16 const * pal)
{
    u32 rgb;
    fi16 r, b, g;

    fu16 fade_r = Unk_03004E20;
    fu16 fade_b = Unk_03003230;
    fu16 fade_g = Unk_03004CE0;

    fu16 fade_colors_beg = 0;
    fu16 fade_colors_end = 0x100;

    fu16 i;

    u8 unk = 0;

    if ((fade_r == 0 && fade_b == 0 && fade_g == 0) || (Unk_030023AC == 0))
    {
        return pal;
    }

    if ((Unk_030023AC & FLAG_UNK_030023AC_01234) == FLAG_UNK_030023AC_4)
    {
        fade_colors_beg = 0x80;
        fade_colors_end = 0x80; // NOTE: is this a bug? maybe they wanted to have 0x80 colors applied making the end 0x100
    }
    else if (!(Unk_030023AC & FLAG_UNK_030023AC_4))
    {
        fade_colors_beg = 0;
        fade_colors_end = 0x80;
    }

    if (fade_colors_beg == 0 && !(Unk_030023AC & FLAG_UNK_030023AC_6))
    {
        // NOTE: do nothing!
    }

    Unk_03005254 = Q4(1.0);
    Unk_030053B4 = Q4(1.0);

    if ((Unk_030023AC & FLAG_UNK_030023AC_6))
    {
        Unk_03005254 = Q4(0.5);
        Unk_030053B4 = Q4(0.5);
    }

    if ((Unk_030023AC & FLAG_UNK_030023AC_7))
    {
        fade_r = ~Unk_03004E20 + 1;
        fade_b = ~Unk_03003230 + 1;
        fade_g = ~Unk_03004CE0 + 1;
    }

    CpuFastCopy(pal, Unk_0200AA10, 0x100 * 2);

    pal += fade_colors_beg;

    for (i = fade_colors_beg; i < fade_colors_end; i++)
    {
        rgb = 0;
        rgb = 0[pal];

        r = (rgb & RGB_MASK_R) >> RGB_SHIFT_R;
        g = (rgb & RGB_MASK_G) >> RGB_SHIFT_G;
        b = (rgb & RGB_MASK_B) >> RGB_SHIFT_B;

        r = MAX(0, ((fi16) Q4_INT(r * Unk_030053B4)) + ((fi16) Q4_INT(fade_r * Unk_03005254)));
        g = MAX(0, ((fi16) Q4_INT(g * Unk_030053B4)) + ((fi16) Q4_INT(fade_g * Unk_03005254)));
        b = MAX(0, ((fi16) Q4_INT(b * Unk_030053B4)) + ((fi16) Q4_INT(fade_b * Unk_03005254)));

        r = MIN(31, r);
        g = MIN(31, g);
        b = MIN(31, b);

        rgb = (r << RGB_SHIFT_R) | ((g << RGB_SHIFT_G) & RGB_MASK_G) | ((b << RGB_SHIFT_B) & RGB_MASK_B);
        Unk_0200AA10[i] = rgb & (RGB_MASK_R | RGB_MASK_G | RGB_MASK_B);

        pal++;
    }

    return Unk_0200AA10;
}

void func_08001678(struct Task * self)
{
    i8 * var_04 = &Unk_03002410.tasks[Unk_03002410.unk_01].unk_03;

    Unk_030054F0 = MAX(0, Unk_030054F0 - 1);

    if (Unk_030054F0 == 0)
    {
        if (SavedGameState.unk_2A9 != 0)
        {
            if (*var_04 == ENUM_FUNC_0800176C_ARG_0)
            {
                m4aMPlayVolumeControl(MusicPlayerTable[SongTable[Unk_0300412C].ms].music_player, TRACKS_ALL, Q8(1.0));
            }
            else if (*var_04 == ENUM_FUNC_0800176C_ARG_1)
            {
                m4aSongNumStartOrContinue(Unk_0300412C);
            }
        }

        Unk_030054F0 = 0;
        Unk_030024A0->unk_00[0] = ENUM_TASK_00_0;
        Unk_03002410.unk_00--;
    }
}

void func_0800176C(fu16 arg_00, fu8 arg_02)
{
    i8 * var_04;

    if (Unk_030054F0 == 0)
    {
        if (SavedGameState.unk_2A9 != 0)
        {
            if (arg_02 == ENUM_FUNC_0800176C_ARG_0)
            {
                m4aMPlayVolumeControl(MusicPlayerTable[SongTable[Unk_0300412C].ms].music_player, TRACKS_ALL, Q8(0.5));
            }
            else if (arg_02 == ENUM_FUNC_0800176C_ARG_1)
            {
                m4aSongNumStop(Unk_0300412C);
            }
        }

        Unk_030024A0 = func_08000570(func_08001678, &Unk_03002410, CONSTANT_UNK_03002410_SLOT_AUTO);
        var_04 = &Unk_030024A0->unk_03;

        *var_04 = arg_02;
        Unk_030054F0 = arg_00;
    }
}

void func_08001838(void)
{
    u32 dummy;
}

void func_08001848(void)
{
    REG_IME = TRUE;
    REG_IE = IRQ_VBLANK | IRQ_CARTRIDGE;
    REG_DISPSTAT = LCD_VBLANK_IRQ_ENABLE;
    Unk_03005C44.unk_02 = 0;
    Unk_03005C44.unk_01 = 0;
}

void func_08001894(void)
{
    Unk_030023C4 |= FLAG_UNK_030023C4_BIT_20;
}

void func_080018B0(void)
{
    Unk_030023C4 &= ~FLAG_UNK_030023C4_BIT_20;
}

void func_080018D0(void)
{
    MainFunc();
}

void func_080018E8(void)
{
    if (MainFunc == OnMain_08001920)
        m4aSoundMain();

    VBlankIntrWait();
}

void func_0800190C(void)
{
    VBlankIntrWait();
    SyncDisp();
}

void OnMain_08001920(void)
{
    m4aSoundMain();
    VBlankIntrWait();
    SyncDisp();
}

MainFuncPtr CONST_DATA Unk_0817B400[] =
{
    func_0800190C,
    OnMain_08001920,
};

void func_08001938(fu8 id)
{
    MainFunc = Unk_0817B400[id];

    if (id == 0)
    {
        IntrTable[IRQ_INDEX_VBLANK] = OnVBlank_08000D68;
    }
    else
    {
        IntrTable[IRQ_INDEX_VBLANK] = OnVBlank_08000DC8;
    }
}

void func_0800198C(struct Task * self)
{
    u16 var_04;
    u16 var_06;

    var_06 = 0;

    Unk_03004EC0 = Unk_02005810;
    Unk_02020FC0[0] = 0;
    Unk_0821646E = 0; // NOTE: write to ROM!!!

    VBlankIntrWait();
    REG_DISPCNT = 0;

    Unk_030055CC = 0;
    Unk_030023C4 &= ~FLAG_UNK_030023AC_5;

    DmaFill32(3, 0, &SavedGameState, sizeof(SavedGameState));

    DmaFill32(3, 0, MEM_VRAM, 0x18000);

    REG_WAITCNT = WAITCNT_ROM_N_3 | WAITCNT_ROM_S_1 | WAITCNT_PREFETCH_ENABLE;
    Unk_030023C4 |= FLAG_UNK_030023AC_2;
    Unk_03004EC8 = 1;

    func_080452F4(Unk_0202B1C0);

    Unk_030023CC = 0;
    DispCnt = 0;
    DispCnt = VIDEO_BG0_ENABLE;
    Unk_03003104 = Unk_030037E0[0].unk_002;
    Unk_03004E10 = Unk_030037E0[0].unk_006;

    VBlankIntrWait();
    REG_DISPCNT = 0;

    if (Unk_030023CC == 0)
    {
        (Unk_03002410.tasks + Unk_03002410.unk_01)->func = func_0800641C;
    }
    else
    {
        Unk_03002594 = 0x17; // TODO: constants
        func_0806A378(0);

        (Unk_03002410.tasks + Unk_03002410.unk_01)->func = func_08001D38;
    }
}

void func_08001B2C(struct Task * self)
{
    if (Unk_03002410.keys_new & KEY_START)
        Unk_030051B0.unk_0A = 1;
}

void func_08001B6C(fu8 arg_0, u32 arg_4)
{
    arg_0 = 0;
}

void func_08001B88(u32 arg_0)
{
}

// idk why this is here but it is
#include "data/mosaic_table.h"

TaskFunc_t CONST_DATA Unk_0817BCC8[] =
{
    func_08001D38,
    func_08001F5C,
    func_0800198C,
    func_08001B98,
};

void func_08001B98(struct Task * self)
{
    Unk_03004108 = 2;
    Unk_030055D0.unk_2A = 0;
    Unk_030055D0.unk_2B = 0;
    Unk_030055D0.unk_2C = 0;
    Unk_030055D0.unk_2D = 0;
    Unk_030055D0.unk_3F = 0;
    Unk_030055D0.unk_40 = 0;
    Unk_030055D0.unk_41 = 0;
    Unk_030055D0.unk_42 = 0;
    Unk_030055D0.unk_31 = 0;
    SavedGameState.unk_2AF = 0;
    SavedGameState.unk_2B0 = 0;
    SavedGameState.unk_2B1 = 0;
    SavedGameState.unk_2B2 = 0;
    Unk_03002378 = 0;
    func_08001848();
    Unk_03003238 = 0;
    func_080621D4(0x100);
    Unk_030051B0.unk_0A = 0;
    Unk_030037E0[0].unk_088 = 0;
    Unk_030055D0.unk_33 = 0;
    Unk_03004E14 = 0;
    Unk_030023C4 &= ~FLAG_UNK_030023AC_7;
    (Unk_03002410.tasks + Unk_03002410.unk_01)->func = Unk_0817BCC8[Unk_03004108];
}

void func_08001D38(struct Task * self)
{
    fu16 var_04; // NOTE: unused
    fu8 i;

    Unk_030023C4 &= ~FLAG_UNK_030023C4_BIT_9;

    if (Unk_03005324 != 0)
    {
        Unk_030023C8 = 8;
        Unk_030023A4 = 8;
        Unk_0300239C = 32;
        Unk_030023D4 = 32;
        Unk_030037E0[0].unk_002 = SavedGameState.unk_006;
        Unk_030037E0[0].unk_006 = SavedGameState.unk_00A;
        Unk_03003104 = SavedGameState.unk_006;
        Unk_03004E10 = SavedGameState.unk_00A;
    }

    if (Unk_030055D0.unk_31 != 4)
    {
        func_08005E38();
    }

    Unk_030023C4 &= ~FLAG_UNK_030023C4_WINDIM;
    Unk_03005324 = 0;
    Unk_03004EB8 = 0x3000;
    Unk_03004ED0 = 0x2000;
    Unk_03004134 = 0xFF;

    func_0804B6BC(Unk_030037E0);

    Unk_03004EC0 = Unk_02005810;

    if (Unk_08181F40 == 0)
    {
        SetFlag_080025E8(0x27);
        func_08068B78(0x06);
    }

    Unk_0300310C = 0;
    Unk_03004108 = 0;

    func_0800299C();

    if (Unk_03003110.unk_A4 == 0)
    {
        for (i = 0; i < 4; i++)
        {
            if (((Unk_030037E0 + i)->unk_094 & 0xFFFF0000) != 0)
            {
                (Unk_030037E0 + i)->unk_094 = 0;
            }
        }
    }

    Unk_030051A8 = 0;
    Unk_03004108 = 1;

    if (Unk_03004E14 == 0 && Unk_03003110.unk_A4 != 0x56 && Unk_03003110.unk_A4 != 0x7F && Unk_03003238 == 0 && Unk_03004144 == 0)
        Unk_03005BA4 = 0;

    Unk_03004144 = 0;

    self->func = func_08001F5C;
}

void func_08001F5C(struct Task * self)
{
    u32 dummy; // NOTE: unused

    Unk_03004EC0 = Unk_03004EE0;

    Unk_030055D0.unk_10++;

    func_08065FAC();
    func_08038AA8();

    if (Unk_03005324 != 0)
    {
        Unk_03004E1C = 0;
        Unk_030055D0.unk_2F = 0;
        self->func = Unk_0817BCC8[Unk_03004108];
        return;
    }

    if (Unk_08181F40 != 0)
        func_08006D20();

    if (Unk_03005324 != 0)
    {
        Unk_03004E1C = 0;
        Unk_030055D0.unk_2F = 0;
        self->func = Unk_0817BCC8[Unk_03004108];
        return;
    }

    if (Unk_0300410C != 0)
    {
        Unk_0300410C = 0;
        return;
    }

    if ((Unk_030055D0.unk_33 & 0x80) != 0)
    {
        func_08002288(self);
        return;
    }
    else if (Unk_030055D0.unk_33 != 0)
    {
        Unk_030055D0.unk_33 = 0;
        Unk_03004108 = 0;
        self->func = Unk_0817BCC8[Unk_03004108];
        return;
    }

    func_0804B718(Unk_030037E0);

    if (Unk_03005324 != 0)
    {
        (Unk_03002410.tasks + Unk_03002410.unk_01)->func = Unk_0817BCC8[Unk_03004108];
        Unk_030055D0.unk_2F = 0;
        Unk_03004E1C = 0;
        return;
    }

    if ((Unk_030055D0.unk_33 & 0x80) != 0)
    {
        func_08002288(self);
        return;
    }
    else if (Unk_030055D0.unk_33 != 0)
    {
        Unk_030055D0.unk_33 = 0;
        Unk_03004108 = 0;
        (Unk_03002410.tasks + Unk_03002410.unk_01)->func = Unk_0817BCC8[Unk_03004108];
        return;
    }

    func_08005C08();
    func_080494A0(Unk_030037E0[0].unk_061);
    func_08099D18();

    if (Unk_03005324 != 0)
    {
        Unk_03004E1C = 0;
        Unk_030055D0.unk_2F = 0;
        self->func = Unk_0817BCC8[Unk_03004108];
        return;
    }

    func_0808144C();
    func_08079B10();
    func_08070E30();
    func_0805DAA4();
    func_08080A84();
    func_08002894(Unk_030055D0.unk_10);
    func_08065C64();
    func_0806BE18(0);
    func_0806AC30();

    if (Unk_03003100 != 0)
    {
        func_080B3A84();
        func_080C448C();
        func_0805B18C();
        Unk_03003100 = 0;
    }

    func_08003DB0();

    (Unk_03002410.tasks + Unk_03002410.unk_01)->func = Unk_0817BCC8[Unk_03004108];
}

void func_08002288(struct Task * self)
{
    func_080C46A0();
    func_080656C8();

    Unk_030055D0.unk_33 = 0;

    func_080049F4();
    func_08003DB0();

    while (TRUE)
    {
        for (;;)
        {
            func_08000314(1);
            func_08065FAC();
            func_08038AA8();

            if (Unk_08181F40 != 0)
                func_08006D20();

            if ((Unk_030055D0.unk_33 & 0x80) != 0)
            {
                Unk_030055D0.unk_33 = 0;
                func_080049F4();
            }

            if ((Unk_030055D0.unk_33 & 0x80) == 0)
            {
                break;
            }
        }

        if (Unk_030055D0.unk_33 == 0)
        {
            func_080494A0(Unk_030037E0[0].unk_061);
            func_08099D18();
            func_0808144C();

            if ((Unk_03004148 & 0x20) == 0)
            {
                func_08079B10();
            }

            func_08070E30();
            func_0805DAA4();
            func_08002894(Unk_030055D0.unk_10);
            func_08065C64();

            if (Unk_03003100 != 0)
            {
                func_080B3A84();
                Unk_03003100 = 0;
            }

            func_08003DB0();
        }
        else
        {
            break;
        }
    }

    Unk_030055D0.unk_33 = 0;

    if (Unk_0300310C != 0xFF)
    {
        func_08003D30(0x10, 0);
    }

    for (;;)
    {
        func_08000314(1);
        func_08065FAC();
        func_080494A0(Unk_030037E0[0].unk_061);
        func_0808144C();

        if ((Unk_03004148 & 0x20) == 0)
        {
            func_08079B10();
        }

        func_08070E30();
        func_0805DAA4();
        func_08002894(Unk_030055D0.unk_10);
        func_08065C64();
        func_08003DB0();

        if (Unk_0300310C != 1)
        {
            break;
        }
    }

    func_08008DE4();
    func_080075F4(7);
    func_080018B0();

    Unk_03003110.unk_4C = 0;
    Unk_03003110.unk_A4 = Unk_03005350.unk_08;
    Unk_030023C8 = 8;
    Unk_030023A4 = 8;
    Unk_0300239C = 32;
    Unk_030023D4 = 32;

    func_080467C4();

    if (Unk_03004144 == 0)
    {
        func_08004B48(Unk_03003110.unk_A4);
    }

    if (Unk_03004108 == 0)
    {
        self->func = Unk_0817BCC8[0];
    }
    else
    {
        func_080049B8();

        if (Unk_03003110.unk_A4 == 0)
        {
            func_080434E8(1);
        }

        func_0806572C();
    }
}

void func_08002500(struct Task * self)
{
    Unk_030054F8 = func_080012FC(Unk_030054F8);
}

fu8 CheckFlag_08002524(fu16 flag)
{
    fu8 bank;

    SHOULD_BE_STATIC u8 const bit_lut[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

    switch (flag)
    {
        case FLAG_100:
            return SavedGameState.unk_4B8 & 1;

        case FLAG_101:
            return SavedGameState.unk_4B8 & 2;

        default:
            bank = flag / CHAR_BIT;
            return SavedGameState.flags[bank] & bit_lut[flag % CHAR_BIT]; // (1 << (flag % CHAR_BIT))
    }
}

void SetFlag_080025E8(fu16 flag)
{
    SHOULD_BE_STATIC u8 const bit_lut[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

    fu8 bank = flag / CHAR_BIT;

    switch (flag)
    {
        case FLAG_100:
            SavedGameState.unk_4B8 |= 1;
            break;

        case FLAG_101:
            SavedGameState.unk_4B8 |= 2;
            break;

        default:
            SavedGameState.flags[bank] |= bit_lut[flag % CHAR_BIT]; // (1 << (flag % CHAR_BIT))
            Unk_03006AF0.unk_0E = SavedGameState.flags[2] | (SavedGameState.flags[3] << CHAR_BIT);
            break;
    }
}

void ClearFlag_08002724(fu16 flag)
{
    SHOULD_BE_STATIC u8 const bit_lut[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

    fu8 bank = flag / CHAR_BIT;

    switch (flag)
    {
        case FLAG_100:
            SavedGameState.unk_4B8 &= ~1;
            break;

        case FLAG_101:
            SavedGameState.unk_4B8 &= ~2;
            break;

        default:
            SavedGameState.flags[bank] &= ~bit_lut[flag % CHAR_BIT]; // (1 << (flag % CHAR_BIT))
            break;
    }
}
