#pragma once

#include "types.h"

#define IOREG(type, addr) (*((type volatile *) (addr)))

// video

#define REG_DISPCNT  IOREG(u16, 0x04000000)
#define REG_DISPSTAT IOREG(u16, 0x04000004)
#define REG_VCOUNT   IOREG(u16 const, 0x04000006)
#define REG_BG0CNT   IOREG(u16, 0x04000008)
#define REG_BG1CNT   IOREG(u16, 0x0400000A)
#define REG_BG2CNT   IOREG(u16, 0x0400000C)
#define REG_BG3CNT   IOREG(u16, 0x0400000E)

enum
{
    LCD_WIDTH     = 240,
    LCD_HEIGHT    = 160,
    LCD_SCANLINES = 228,
};

enum
{
    #define VIDEO_MODE(n) (((n) & 7) << 0)

    VIDEO_MODE_REGULAR        = VIDEO_MODE(0),
    VIDEO_MODE_MIXED          = VIDEO_MODE(1),
    VIDEO_MODE_AFFINE         = VIDEO_MODE(2),
    VIDEO_MODE_BITMAP         = VIDEO_MODE(3),
    VIDEO_MODE_BITMAP_INDEXED = VIDEO_MODE(4),
    VIDEO_MODE_BITMAP_SMALL   = VIDEO_MODE(5),

    VIDEO_FRAME_SELECT        = 1 << 4,

    VIDEO_OBJ_MAPPING_1D      = 1 << 6,
    VIDEO_OBJ_MAPPING_2D      = !VIDEO_OBJ_MAPPING_1D,

    VIDEO_FORCE_BLANK         = 1 << 7,

    VIDEO_BG0_ENABLE          = 1 << 8,
    VIDEO_BG1_ENABLE          = 1 << 9,
    VIDEO_BG2_ENABLE          = 1 << 10,
    VIDEO_BG3_ENABLE          = 1 << 11,
    VIDEO_OBJ_ENABLE          = 1 << 12,
    VIDEO_WIN0_ENABLE         = 1 << 13,
    VIDEO_WIN1_ENABLE         = 1 << 14,
    VIDEO_OBJ_WIN_ENABLE      = 1 << 15,
};

enum
{
    LCD_IN_VBLANK         = 1 << 0,
    LCD_IN_HBLANK         = 1 << 1,
    LCD_VCOUNT_MATCH      = 1 << 2,

    LCD_VBLANK_IRQ_ENABLE = 1 << 3,
    LCD_HBLANK_IRQ_ENABLE = 1 << 4,
    LCD_VCOUNT_IRQ_ENABLE = 1 << 5,

    #define LCD_VCOUNT(n) (((n) & 0xFF) << 8)
};

enum
{
    VCOUNT_DRAW_START    = 0,
    VCOUNT_DRAW_END      = 160,
    VCOUNT_BLANK_START   = 160,
    VCOUNT_BLANK_END     = 0,
};

enum
{
    #define BGCNT_PRIORITY(n) (((n) & 3) << 0)

    BGCNT_PRIORITY_MIN          = BGCNT_PRIORITY(3),
    BGCNT_PRIORITY_MAX          = BGCNT_PRIORITY(0),

    #define BGCNT_GFX_BASE(n) (((n) & 3) << 2)

    BGCNT_MOSAIC_ENABLE         = 1 << 6,

    BGCNT_TILE_8BPP             = 1 << 7,
    BGCNT_TILE_4BPP             = !BGCNT_TILE_8BPP,

    #define BGCNT_MAP_BASE(n) (((n) & 31) << 8)

    BGCNT_AFFINE_WRAP           = 1 << 13,

    #define BGCNT_SIZE(n) (((n) & 3) << 14)

    BGCNT_SIZE_256x256          = BGCNT_SIZE(0),
    BGCNT_SIZE_512x256          = BGCNT_SIZE(1),
    BGCNT_SIZE_256x512          = BGCNT_SIZE(2),
    BGCNT_SIZE_512x512          = BGCNT_SIZE(3),

    #define BGCNT_AFFINE_SIZE(n) (((n) & 3) << 14)

    BGCNT_AFFINE_SIZE_128x128   = BGCNT_AFFINE_SIZE(0),
    BGCNT_AFFINE_SIZE_256x256   = BGCNT_AFFINE_SIZE(1),
    BGCNT_AFFINE_SIZE_512x512   = BGCNT_AFFINE_SIZE(2),
    BGCNT_AFFINE_SIZE_1024x1024 = BGCNT_AFFINE_SIZE(3),
};

// irq

#define REG_IE  IOREG(u16, 0x04000200)
#define REG_IF  IOREG(u16, 0x04000202)
#define REG_IME IOREG(u16, 0x04000208)

enum
{
    IRQ_VBLANK          = 1 << 0,
    IRQ_HBLANK          = 1 << 1,
    IRQ_VCOUNT          = 1 << 2,
    IRQ_TIMER_0         = 1 << 3,
    IRQ_TIMER_1         = 1 << 4,
    IRQ_TIMER_2         = 1 << 5,
    IRQ_TIMER_3         = 1 << 6,
    IRQ_SERIAL          = 1 << 7,
    IRQ_DMA_0           = 1 << 8,
    IRQ_DMA_1           = 1 << 9,
    IRQ_DMA_2           = 1 << 10,
    IRQ_DMA_3           = 1 << 11,
    IRQ_KEYPAD          = 1 << 12,
    IRQ_CARTRIDGE       = 1 << 13,
};

// waitstates

#define REG_WAITCNT IOREG(u16, 0x04000204)

#define WAITCNT_SRAM(n)    (((n) & 3) << 0)
#define WAITCNT_ROM_N(n)   (((n) & 3) << 2)
#define WAITCNT_ROM_S(n)   (((n) & 1) << 4)
#define WAITCNT_PHI_OUT(n) (((n) & 3) << 11)

enum
{
    WAITCNT_SRAM_4          = WAITCNT_SRAM(0),
    WAITCNT_SRAM_3          = WAITCNT_SRAM(1),
    WAITCNT_SRAM_2          = WAITCNT_SRAM(2),
    WAITCNT_SRAM_8          = WAITCNT_SRAM(3),
    WAITCNT_SRAM_MASK       = WAITCNT_SRAM(3),

    WAITCNT_ROM_N_4         = WAITCNT_ROM_N(0),
    WAITCNT_ROM_N_3         = WAITCNT_ROM_N(1),
    WAITCNT_ROM_N_2         = WAITCNT_ROM_N(2),
    WAITCNT_ROM_N_8         = WAITCNT_ROM_N(3),
    WAITCNT_ROM_N_MASK      = WAITCNT_ROM_N(3),

    WAITCNT_ROM_S_2         = WAITCNT_ROM_S(0),
    WAITCNT_ROM_S_1         = WAITCNT_ROM_S(1),
    WAITCNT_ROM_S_MASK      = WAITCNT_ROM_S(1),

    WAITCNT_PHI_OUT_DISABLE = WAITCNT_PHI_OUT(0),
    WAITCNT_PHI_OUT_4MHZ    = WAITCNT_PHI_OUT(1),
    WAITCNT_PHI_OUT_8MHZ    = WAITCNT_PHI_OUT(2),
    WAITCNT_PHI_OUT_16MHZ   = WAITCNT_PHI_OUT(3),
    WAITCNT_PHI_OUT_MASK    = WAITCNT_PHI_OUT(3),

    WAITCNT_PREFETCH_ENABLE = 1 << 14,
};

// dma

#define REG_DMA0SRC   IOREG(void const *, 0x040000B0)
#define REG_DMA0DST   IOREG(void *, 0x040000B4)
#define REG_DMA0CNT   IOREG(u32, 0x040000B8)
#define REG_DMA0CNT_L IOREG(u16, 0x040000B8)
#define REG_DMA0CNT_H IOREG(u16, 0x040000BA)

#define REG_DMA1SRC   IOREG(void const *, 0x040000BC)
#define REG_DMA1DST   IOREG(void *, 0x040000C0)
#define REG_DMA1CNT   IOREG(u32, 0x040000C4)
#define REG_DMA1CNT_L IOREG(u16, 0x040000C4)
#define REG_DMA1CNT_H IOREG(u16, 0x040000C6)

#define REG_DMA2SRC   IOREG(void const *, 0x040000C8)
#define REG_DMA2DST   IOREG(void *, 0x040000CC)
#define REG_DMA2CNT   IOREG(u32, 0x040000D0)
#define REG_DMA2CNT_L IOREG(u16, 0x040000D0)
#define REG_DMA2CNT_H IOREG(u16, 0x040000D2)

#define REG_DMA3SRC   IOREG(void const *, 0x040000D4)
#define REG_DMA3DST   IOREG(void *, 0x040000D8)
#define REG_DMA3CNT   IOREG(u32, 0x040000DC)
#define REG_DMA3CNT_L IOREG(u16, 0x040000DC)
#define REG_DMA3CNT_H IOREG(u16, 0x040000DE)

#define DMA_DST(n) (((n) & 3) << 21)
#define DMA_SRC(n) (((n) & 3) << 23)
#define DMA_START(n) (((n) & 3) << 28)

enum
{
    DMA_DST_INCREMENT   = DMA_DST(0),
    DMA_DST_DECREMENT   = DMA_DST(1),
    DMA_DST_FIXED       = DMA_DST(2),
    DMA_DST_RELOAD      = DMA_DST(3),

    DMA_SRC_INCREMENT   = DMA_SRC(0),
    DMA_SRC_DECREMENT   = DMA_SRC(1),
    DMA_SRC_FIXED       = DMA_SRC(2),

    DMA_REPEAT          = 1 << 25,

    DMA_32BIT           = 1 << 26,
    DMA_16BIT           = !DMA_32BIT,

    DMA_START_NOW       = DMA_START(0),
    DMA_START_HBLANK    = DMA_START(1),
    DMA_START_VBLANK    = DMA_START(2),
    DMA_START_SPECIAL   = DMA_START(3),

    // DMA1, DMA2
    DMA_START_SOUND     = DMA_START_SPECIAL,
    // DMA3
    DMA_START_CAPTURE   = DMA_START_SPECIAL,

    // On transfer completion
    DMA_IRQ_ENABLE      = 1 << 30,
    DMA_ENABLE          = 1 << 31,
};

// timer

#define REG_TM0VAL      IOREG(u16, 0x04000100)
#define REG_TM0CNT      IOREG(u16, 0x04000102)
#define REG_TM1VAL      IOREG(u16, 0x04000104)
#define REG_TM1CNT      IOREG(u16, 0x04000106)
#define REG_TM2VAL      IOREG(u16, 0x04000108)
#define REG_TM2CNT      IOREG(u16, 0x0400010A)
#define REG_TM3VAL      IOREG(u16, 0x0400010C)
#define REG_TM3CNT      IOREG(u16, 0x0400010E)

#define TIMER_FREQ(n) (((n) & 7) << 0)

enum
{
    TIMER_FREQ_16MHZ    = TIMER_FREQ(0),
    TIMER_FREQ_262KHZ   = TIMER_FREQ(1),
    TIMER_FREQ_64KHZ    = TIMER_FREQ(2),
    TIMER_FREQ_16KHZ    = TIMER_FREQ(3),
    TIMER_FREQ_CASCADE  = TIMER_FREQ(4),
    TIMER_IRQ_ENABLE    = 1 << 6,
    TIMER_ENABLE        = 1 << 7,
};

// sound

#define REG_SOUND1CNT_L IOREG(u16, 0x04000060)
#define REG_NR10        IOREG(u8,  0x04000060)
#define REG_SOUND1CNT_H IOREG(u16, 0x04000062)
#define REG_NR11        IOREG(u8,  0x04000062)
#define REG_NR12        IOREG(u8,  0x04000063)
#define REG_SOUND1CNT_X IOREG(u16, 0x04000064)
#define REG_NR13        IOREG(u8,  0x04000064)
#define REG_NR14        IOREG(u8,  0x04000065)
#define REG_SOUND2CNT_L IOREG(u16, 0x04000068)
#define REG_NR21        IOREG(u8,  0x04000068)
#define REG_NR22        IOREG(u8,  0x04000069)
#define REG_SOUND2CNT_H IOREG(u16, 0x0400006C)
#define REG_NR23        IOREG(u8,  0x0400006C)
#define REG_NR24        IOREG(u8,  0x0400006D)
#define REG_SOUND3CNT_L IOREG(u16, 0x04000070)
#define REG_NR30        IOREG(u8,  0x04000070)
#define REG_SOUND3CNT_H IOREG(u16, 0x04000072)
#define REG_NR31        IOREG(u8,  0x04000072)
#define REG_NR32        IOREG(u8,  0x04000073)
#define REG_SOUND3CNT_X IOREG(u16, 0x04000074)
#define REG_NR33        IOREG(u8,  0x04000074)
#define REG_NR34        IOREG(u8,  0x04000075)
#define REG_SOUND4CNT_L IOREG(u16, 0x04000078)
#define REG_NR41        IOREG(u8,  0x04000078)
#define REG_NR42        IOREG(u8,  0x04000079)
#define REG_SOUND4CNT_H IOREG(u16, 0x0400007C)
#define REG_NR43        IOREG(u8,  0x0400007C)
#define REG_NR44        IOREG(u8,  0x0400007D)
#define REG_SOUNDCNT_L  IOREG(u16, 0x04000080)
#define REG_NR50        IOREG(u8,  0x04000080)
#define REG_NR51        IOREG(u8,  0x04000081)
#define REG_SOUNDCNT_H  IOREG(u16, 0x04000082)
#define REG_SOUNDCNT_X  IOREG(u16, 0x04000084)
#define REG_NR52        IOREG(u8,  0x04000084)
#define REG_SOUNDBIAS   IOREG(u16, 0x04000088)
#define REG_SOUNDBIAS_L IOREG(u8,  0x04000088)
#define REG_SOUNDBIAS_H IOREG(u8,  0x04000089)
#define REG_WAVE_RAM0   IOREG(u32, 0x04000090)
#define REG_WAVE_RAM1   IOREG(u32, 0x04000094)
#define REG_WAVE_RAM2   IOREG(u32, 0x04000098)
#define REG_WAVE_RAM3   IOREG(u32, 0x0400009C)
#define REG_FIFO_A      IOREG(u32, 0x040000A0)
#define REG_FIFO_B      IOREG(u32, 0x040000A4)

enum
{
    // SOUNDCNT_H
    SOUND_CGB_MIX_QUARTER = 0x0000,
    SOUND_CGB_MIX_HALF    = 0x0001,
    SOUND_CGB_MIX_FULL    = 0x0002,
    SOUND_A_MIX_HALF      = 0x0000,
    SOUND_A_MIX_FULL      = 0x0004,
    SOUND_B_MIX_HALF      = 0x0000,
    SOUND_B_MIX_FULL      = 0x0008,
    SOUND_ALL_MIX_FULL    = 0x000E,
    SOUND_A_RIGHT_OUTPUT  = 0x0100,
    SOUND_A_LEFT_OUTPUT   = 0x0200,
    SOUND_A_TIMER_0       = 0x0000,
    SOUND_A_TIMER_1       = 0x0400,
    SOUND_A_FIFO_RESET    = 0x0800,
    SOUND_B_RIGHT_OUTPUT  = 0x1000,
    SOUND_B_LEFT_OUTPUT   = 0x2000,
    SOUND_B_TIMER_0       = 0x0000,
    SOUND_B_TIMER_1       = 0x4000,
    SOUND_B_FIFO_RESET    = 0x8000,
};

enum
{
    // SOUNDCNT_X
    SOUND_1_ON          = 0x0001,
    SOUND_2_ON          = 0x0002,
    SOUND_3_ON          = 0x0004,
    SOUND_4_ON          = 0x0008,
    SOUND_MASTER_ENABLE = 0x0080,
};
