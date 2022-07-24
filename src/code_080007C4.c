#include "types.h"
#include "macros.h"
#include "agbio.h"
#include "agbsyscalls.h"
#include "m4a.h"

#include "attributes.h" // STRUCT_PAD

#include "unknown.h"

// TODO: this should probably by a hardwired address rather than a symbol
extern IntrMainPtr Unk_03007FFC;

// TODO: this should probably by a hardwired address rather than a symbol
extern u16 Unk_03007FF8;

static u32 IntrMainRam[0x200];

void IntrInit(void)
{
    fu8 i;

    for (i = 0; i <= IRQ_INDEX_MAX; i++)
        IntrTable[i] = IntrTableTemplate[i];

    DmaCopy16(3, IntrMain, IntrMainRam, sizeof(IntrMainRam));
    Unk_03007FFC = (IntrMainPtr) IntrMainRam;

    REG_IME = TRUE;
    REG_IE = IRQ_VBLANK | IRQ_CARTRIDGE;
    REG_DISPSTAT = LCD_VBLANK_IRQ_ENABLE;

    Unk_0300249C = Unk_0817B400[0];
    IntrTable[IRQ_INDEX_VBLANK] = OnVBlank_08000D68;
}

enum
{
    FLAG_UNK_03004ED8_0 = 1 << 0,
};

enum
{
    FLAG_UNK_030023C4_BIT_0 = 1 << 0,
    FLAG_UNK_030023C4_WINDIM = 1 << 3,

    FLAG_UNK_030023C4_INHDMA = 1 << 10,

    FLAG_UNK_030023C4_BG0TM = 1 << 16,
    FLAG_UNK_030023C4_BG1TM = 1 << 17,
    FLAG_UNK_030023C4_BG2TM = 1 << 18,
    FLAG_UNK_030023C4_BG3TM = 1 << 19,

    FLAG_UNK_030023C4_BIT_20 = 1 << 20,

    FLAG_UNK_030023C4_ANYTM = FLAG_UNK_030023C4_BG0TM | FLAG_UNK_030023C4_BG1TM | FLAG_UNK_030023C4_BG2TM | FLAG_UNK_030023C4_BG3TM,
};

enum
{
    RGB_SHIFT_R = 0,
    RGB_SHIFT_G = 5,
    RGB_SHIFT_B = 10,
};

enum
{
    RGB_MASK_R = 0x1F << RGB_SHIFT_R,
    RGB_MASK_G = 0x1F << RGB_SHIFT_G,
    RGB_MASK_B = 0x1F << RGB_SHIFT_B,
};

enum
{
    FLAG_UNK_030023AC_0 = (1 << 0),
    FLAG_UNK_030023AC_1 = (1 << 1),
    FLAG_UNK_030023AC_2 = (1 << 2),
    FLAG_UNK_030023AC_3 = (1 << 3),
    FLAG_UNK_030023AC_4 = (1 << 4),
    FLAG_UNK_030023AC_5 = (1 << 5),
    FLAG_UNK_030023AC_6 = (1 << 6),
    FLAG_UNK_030023AC_7 = (1 << 7),

    FLAG_UNK_030023AC_01234 = FLAG_UNK_030023AC_0 | FLAG_UNK_030023AC_1 | FLAG_UNK_030023AC_2 | FLAG_UNK_030023AC_3 | FLAG_UNK_030023AC_4,
};

enum
{
    ENUM_FUNC_0800176C_ARG_0,
    ENUM_FUNC_0800176C_ARG_1,
};

struct Unk_03005350
{
    /* 00 */ u8 unk_00;
    /* 01 */ u8 unk_01;
    /* 02 */ u8 unk_02;
    /* 03 */ STRUCT_PAD(0x03, 0x04);
    /* 04 */ u8 unk_04;
    /* 05 */ STRUCT_PAD(0x05, 0x08);
    /* 08 */ u16 unk_08;
    /* 0A */ STRUCT_PAD(0x0A, 0x0C);
    /* 0C */ u32 unk_0C;
    /* 10 */ u8 unk_10;
    /* 11 */ u8 unk_11;
    /* 12 */ i16 pa, pb, pc, pd;
    /* 1A */ STRUCT_PAD(0x1A, 0x1C);
    /* 1C */ i32 x, y;
    /* 24 */ STRUCT_PAD(0x24, 0x25);
    /* 25 */ u8 unk_25;
    /* 26 */ STRUCT_PAD(0x26, 0x5B);
    /* 5B */ u8 unk_5B;
};

struct Unk_03005C44
{
    /* 00 */ u8 unk_00; // "hdma" chunk len
    /* 01 */ u8 unk_01; // other "hdma" chunk len
    /* 02 */ u8 unk_02; // ?
};

struct Unk_030055D0
{
    /* 00 */ STRUCT_PAD(0x00, 0x2A);
    /* 2A */ i8 unk_2A;
};

struct Unk_0202DBD0
{
    /* 000 */ STRUCT_PAD(0x000, 0x2A9);
    /* 2A9 */ u8 unk_2A9; // sound/music enable?
    /* 2AA */ STRUCT_PAD(0x2AA, 0x500);
};

struct Unk_030051B0
{
    /* 00 */ STRUCT_PAD(0x00, 0x0A);
    /* 0A */ u8 unk_0A;
};

struct Unk_030037E0
{
    /* 00 */ STRUCT_PAD(0x00, 0x02);
    /* 02 */ u16 unk_02;
    /* 04 */ STRUCT_PAD(0x04, 0x06);
    /* 06 */ u16 unk_06;
};

extern void func_08000E24(void);
extern void func_08001050(u8 * unused_0);
extern void func_08001188(void);
extern void func_08001228(void);
extern void func_08001920(void);
extern void func_08001D38(struct Task * self);
extern void func_0800641C(struct Task * self);
extern void func_080452F4(void * unk);
extern void func_08047848(void * unk);
extern void func_080682C0(void);
extern void func_08068300(void);
extern void func_0806A378(u16 arg_0);
extern void func_0806AA04(u8 arg_0);
extern void func_08072E3C(void);

extern u16 Unk_030024A8;
extern u8 Unk_03004ED8;
extern u16 Unk_03002594; // input for func_0806AA04
extern u16 Unk_030023A0; // some DispControl
extern u16 Unk_03002374; // some BlendVal
extern u8 Unk_030023AC; // some color fade flags
extern u8 Unk_03004140;
extern u8 Unk_02020FC0[8];
extern u32 Unk_030023C4; // some flags?
extern void * Unk_03004EC0;
extern u16 Unk_03002350; // Bg0Cnt
extern u16 Unk_03002370; // Bg1Cnt
extern u16 Unk_03002400; // Bg2Cnt
extern u16 Unk_030023A8; // Bg3Cnt
extern u16 Unk_030023B0; // Bg0HOffset
extern u16 Unk_030023F0; // Bg0VOffset
extern u16 Unk_0300237C; // Bg1HOffset
extern u16 Unk_03002394; // Bg1VOffset
extern u16 Unk_03002384; // Bg2HOffset
extern u16 Unk_030023B4; // Bg2VOffset
extern u16 Unk_030023FC; // Bg3HOffset
extern u16 Unk_03002398; // Bg3VOffset
extern i32 Unk_0300238C; // some Bg2X
extern i32 Unk_03002388; // some Bg2Y
extern u16 Unk_03004104; // BgExtraHOffset
extern u16 Unk_03004E24; // BgExtraVOffset
extern u8 Unk_030023C8; // some BgHOffsetAddend
extern u8 Unk_0300239C; // some BgVOffsetAddend
extern u8 Unk_03004E14; // some bool?
extern struct Unk_03005350 Unk_03005350;
extern u16 Unk_030023BC; // some BlendCnt
extern u16 Unk_030023D8; // some BlendAlpha
extern u16 Unk_0300559C; // some Mosaic
extern u16 Unk_030023B8; // some WinIn
extern u16 Unk_030023F4; // some WinOut
extern u16 Unk_03002408; // some Win0H
extern u16 Unk_030023F8; // some Win0V
extern u16 Unk_03002354; // some Win1H
extern u16 Unk_030023DC; // some Win1V
extern int Unk_0202D2C0;
extern u8 Unk_03004E1C; // some bool?
extern void const * Unk_02019E00[];
extern void * Unk_0200B820[];
extern u16 Unk_02019FC0[0x400]; // Bg0Tm
extern u16 Unk_0201A7C0[0x400]; // Bg1Tm
extern u16 Unk_0201AFC0[0x400]; // Bg2Tm
extern u16 Unk_0201B7C0[0x400]; // Bg3Tm
extern u8 Unk_02011DA0[0xA00]; // "hdma" buffer
extern u16 Unk_030024A8; // "hdma" offset
extern struct Unk_03005C44 Unk_03005C44;
extern void * Unk_03005C3C; // "hdma" dst
extern u8 Unk_03005178;
extern u16 const * Unk_030054F8; // palette transfer buf
extern u8 Unk_03004EBC; // palette transfer id
extern u16 Unk_03005C50; // palette transfer length
extern u8 Unk_0200BE80[]; // vdma buf
extern void * Unk_03002390; // vdma dst
extern u16 Unk_03002378; // vdma length
extern void const * Unk_03002380; // vdma src
extern u8 Unk_020213C0[]; // another vdma buf?
extern u8 Unk_030023CC;
extern struct Unk_030055D0 Unk_030055D0;
extern u8 Unk_020127A0[0xA00]; // other "hdma" buffer
extern u32 Unk_03004E20; // some red
extern u32 Unk_03003230; // some blue
extern u32 Unk_03004CE0; // some green
extern u16 Unk_0200AA10[]; // some colors
extern u8 Unk_03005254; // some intensity q4
extern u8 Unk_030053B4; // some intensity q4
extern u16 Unk_030054F0; // some sound related counter
extern struct Unk_0202DBD0 Unk_0202DBD0; // big struct. user options?
extern struct Task * Unk_030024A0; // sound related task
extern u8 Unk_0300412C; // some song id
extern u8 Unk_02005810[]; // ?
extern u8 CONST_DATA Unk_0821646E;
extern u8 Unk_030055CC; // ?
extern u16 Unk_03004EC8; // ?
extern u8 Unk_0202B1C0[]; // ?
extern struct Unk_030037E0 Unk_030037E0; // ?
extern u16 Unk_03003104; // ?
extern u16 Unk_03004E10; // ?
extern struct Unk_030051B0 Unk_030051B0; // ?

void SyncDisp(void)
{
    Unk_030024A8 = 0;

    if (Unk_03004ED8 & FLAG_UNK_03004ED8_0)
    {
        func_08001228();
        func_0806AA04(Unk_03002594);
        REG_DISPCNT = Unk_030023A0;
        REG_BLDVAL = Unk_03002374;
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

    REG_BG0CNT = Unk_03002350;
    REG_BG1CNT = Unk_03002370;
    REG_BG2CNT = Unk_03002400;
    REG_BG3CNT = Unk_030023A8;

    if (Unk_030023C4 & FLAG_UNK_030023C4_BIT_20)
    {
        REG_BG0HOFS = Unk_030023B0;
        REG_BG0VOFS = Unk_030023F0;
        REG_BG1HOFS = Unk_0300237C + Unk_03004104;
        REG_BG1VOFS = Unk_03002394 + Unk_03004E24;
        REG_BG2HOFS = Unk_03002384 + Unk_03004104;
        REG_BG2VOFS = Unk_030023B4 + Unk_03004E24;
        REG_BG3HOFS = Unk_030023FC;
        REG_BG3VOFS = Unk_03002398;
        REG_BG2X = Unk_0300238C;
        REG_BG2Y = Unk_03002388;
    }
    else
    {
        REG_BG0HOFS = Unk_030023B0 + Unk_030023C8 + Unk_03004104;
        REG_BG0VOFS = Unk_030023F0 + Unk_0300239C + Unk_03004E24;
        REG_BG1HOFS = Unk_0300237C + Unk_030023C8 + Unk_03004104;
        REG_BG1VOFS = Unk_03002394 + Unk_0300239C + Unk_03004E24;
        REG_BG2HOFS = Unk_03002384 + Unk_030023C8 + Unk_03004104;
        REG_BG2VOFS = Unk_030023B4 + Unk_0300239C + Unk_03004E24;
        REG_BG3HOFS = Unk_030023FC + Unk_030023C8 + Unk_03004104;
        REG_BG3VOFS = Unk_03002398 + Unk_0300239C + Unk_03004E24;
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

    REG_BLDCNT = Unk_030023BC;
    REG_BLDALPHA = Unk_030023D8;
    REG_MOSAIC = Unk_0300559C;
    REG_WININ = Unk_030023B8;
    REG_WINOUT = Unk_030023F4;

    if (Unk_030023C4 & FLAG_UNK_030023C4_WINDIM)
    {
        REG_WIN0H = Unk_03002408;
        REG_WIN0V = Unk_030023F8;
        REG_WIN1H = Unk_03002354;
        REG_WIN1V = Unk_030023DC;
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
    Unk_03007FF8 |= IRQ_VBLANK;

    func_08001188();

    Unk_030024A8 = 0;

    m4aSoundVSync();
    m4aSoundMain();

    Unk_03005350.unk_5B++;
}

void OnVBlank_08000DC8(void)
{
    Unk_03007FF8 |= IRQ_VBLANK;

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

    Unk_03007FF8 = IRQ_HBLANK;
}

void func_08001044(void)
{
}

void func_08001050(u8 * unused_0)
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
        Unk_0200AA10[i] = rgb & 0x7FFF;

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
        if (Unk_0202DBD0.unk_2A9 != 0)
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
        Unk_030024A0->unk_00[0] = ENUM_UNK_03002410_08_00_0;
        Unk_03002410.unk_00--;
    }
}

void func_0800176C(u16 arg_00, u8 arg_02)
{
    i8 * var_04;

    if (Unk_030054F0 == 0)
    {
        if (Unk_0202DBD0.unk_2A9 != 0)
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
    Unk_0300249C();
}

void func_080018E8(void)
{
    if (Unk_0300249C == func_08001920)
        m4aSoundMain();

    VBlankIntrWait();
}

void func_0800190C(void)
{
    VBlankIntrWait();
    SyncDisp();
}

void func_08001920(void)
{
    m4aSoundMain();
    VBlankIntrWait();
    SyncDisp();
}

void func_08001938(u8 id)
{
    Unk_0300249C = Unk_0817B400[id];

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

    DmaFill32(3, 0, &Unk_0202DBD0, sizeof(Unk_0202DBD0));

    DmaFill32(3, 0, MEM_VRAM, 0x18000);

    REG_WAITCNT = WAITCNT_ROM_N_3 | WAITCNT_ROM_S_1 | WAITCNT_PREFETCH_ENABLE;
    Unk_030023C4 |= FLAG_UNK_030023AC_2;
    Unk_03004EC8 = 1;

    func_080452F4(Unk_0202B1C0);

    Unk_030023CC = 0;
    Unk_030023A0 = 0;
    Unk_030023A0 = VIDEO_BG0_ENABLE;
    Unk_03003104 = Unk_030037E0.unk_02;
    Unk_03004E10 = Unk_030037E0.unk_06;

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

void func_08001B6C(u8 arg_0, u32 arg_4)
{
    arg_0 = 0;
}

void func_08001B88(u32 arg_0)
{
}
