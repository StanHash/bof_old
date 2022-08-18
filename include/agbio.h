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

// video bg scroll

#define REG_BG0HOFS IOREG(u16, 0x04000010)
#define REG_BG0VOFS IOREG(u16, 0x04000012)
#define REG_BG1HOFS IOREG(u16, 0x04000014)
#define REG_BG1VOFS IOREG(u16, 0x04000016)
#define REG_BG2HOFS IOREG(u16, 0x04000018)
#define REG_BG2VOFS IOREG(u16, 0x0400001A)
#define REG_BG3HOFS IOREG(u16, 0x0400001C)
#define REG_BG3VOFS IOREG(u16, 0x0400001E)

// video bg affine

#define REG_BG2PA IOREG(i16, 0x04000020)
#define REG_BG2PB IOREG(i16, 0x04000022)
#define REG_BG2PC IOREG(i16, 0x04000024)
#define REG_BG2PD IOREG(i16, 0x04000026)

#define REG_BG2X  IOREG(i32, 0x04000028)
#define REG_BG2Y  IOREG(i32, 0x0400002C)

#define REG_BG3PA IOREG(i16, 0x04000030)
#define REG_BG3PB IOREG(i16, 0x04000032)
#define REG_BG3PC IOREG(i16, 0x04000034)
#define REG_BG3PD IOREG(i16, 0x04000036)

#define REG_BG3X  IOREG(i32, 0x04000038)
#define REG_BG3Y  IOREG(i32, 0x0400003C)

// video window

#define REG_WIN0H  IOREG(u16, 0x04000040)
#define REG_WIN1H  IOREG(u16, 0x04000042)
#define REG_WIN0V  IOREG(u16, 0x04000044)
#define REG_WIN1V  IOREG(u16, 0x04000046)
#define REG_WININ  IOREG(u16, 0x04000048)
#define REG_WINOUT IOREG(u16, 0x0400004A)

enum
{
    WINDOW_BG0_ENABLE   = 1 << 0,
    WINDOW_BG1_ENABLE   = 1 << 1,
    WINDOW_BG2_ENABLE   = 1 << 2,
    WINDOW_BG3_ENABLE   = 1 << 3,
    WINDOW_OBJ_ENABLE   = 1 << 4,
    WINDOW_BLEND_ENABLE = 1 << 5,
};

// video mosaic

#define REG_MOSAIC IOREG(u16, 0x0400004C)

#define MOSAIC_BG_H(v) (((v) & 0xF) << 0)
#define MOSAIC_BG_V(v) (((v) & 0xF) << 4)
#define MOSAIC_OBJ_H(v) (((v) & 0xF) << 8)
#define MOSAIC_OBJ_V(v) (((v) & 0xF) << 12)

// video blend

#define REG_BLDCNT   IOREG(u16, 0x04000050)
#define REG_BLDALPHA IOREG(u16, 0x04000052)
#define REG_BLDVAL   IOREG(u16, 0x04000054)

#define BLEND_MODE(n) (((n) & 3) << 6)

enum
{
    BLEND_TARGET_BG0      = 1 << 0,
    BLEND_TARGET_BG1      = 1 << 1,
    BLEND_TARGET_BG2      = 1 << 2,
    BLEND_TARGET_BG3      = 1 << 3,
    BLEND_TARGET_OBJ      = 1 << 4,
    BLEND_TARGET_BD       = 1 << 5,

    BLEND_MODE_NONE       = BLEND_MODE(0),
    BLEND_MODE_ALPHA      = BLEND_MODE(1),
    BLEND_MODE_WHITE      = BLEND_MODE(2),
    BLEND_MODE_BLACK      = BLEND_MODE(3),

    // Only used with BLEND_MODE_ALPHA
    BLEND_TARGET2_BG0     = 1 << 8,
    BLEND_TARGET2_BG1     = 1 << 9,
    BLEND_TARGET2_BG2     = 1 << 10,
    BLEND_TARGET2_BG3     = 1 << 11,
    BLEND_TARGET2_OBJ     = 1 << 12,
    BLEND_TARGET2_BD      = 1 << 13,
};

// irq

#define REG_IE  IOREG(u16, 0x04000200)
#define REG_IF  IOREG(u16, 0x04000202)
#define REG_IME IOREG(u16, 0x04000208)

enum
{
    // Bit indices of IRQs
    IRQ_INDEX_VBLANK,
    IRQ_INDEX_HBLANK,
    IRQ_INDEX_VCOUNT,
    IRQ_INDEX_TIMER_0,
    IRQ_INDEX_TIMER_1,
    IRQ_INDEX_TIMER_2,
    IRQ_INDEX_TIMER_3,
    IRQ_INDEX_SERIAL,
    IRQ_INDEX_DMA_0,
    IRQ_INDEX_DMA_1,
    IRQ_INDEX_DMA_2,
    IRQ_INDEX_DMA_3,
    IRQ_INDEX_KEYPAD,
    IRQ_INDEX_CARTRIDGE,
    IRQ_INDEX_UNK,
    IRQ_INDEX_MAX,
};

enum
{
    IRQ_VBLANK          = 1 << IRQ_INDEX_VBLANK,
    IRQ_HBLANK          = 1 << IRQ_INDEX_HBLANK,
    IRQ_VCOUNT          = 1 << IRQ_INDEX_VCOUNT,
    IRQ_TIMER_0         = 1 << IRQ_INDEX_TIMER_0,
    IRQ_TIMER_1         = 1 << IRQ_INDEX_TIMER_1,
    IRQ_TIMER_2         = 1 << IRQ_INDEX_TIMER_2,
    IRQ_TIMER_3         = 1 << IRQ_INDEX_TIMER_3,
    IRQ_SERIAL          = 1 << IRQ_INDEX_SERIAL,
    IRQ_DMA_0           = 1 << IRQ_INDEX_DMA_0,
    IRQ_DMA_1           = 1 << IRQ_INDEX_DMA_1,
    IRQ_DMA_2           = 1 << IRQ_INDEX_DMA_2,
    IRQ_DMA_3           = 1 << IRQ_INDEX_DMA_3,
    IRQ_KEYPAD          = 1 << IRQ_INDEX_KEYPAD,
    IRQ_CARTRIDGE       = 1 << IRQ_INDEX_CARTRIDGE,
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

// input

#define REG_KEYINPUT IOREG(u16 const, 0x04000130)
#define REG_KEYCNT   IOREG(u16, 0x04000132)

enum
{
    // Bit indices of keys bits.
    KEY_INDEX_A,
    KEY_INDEX_B,
    KEY_INDEX_SELECT,
    KEY_INDEX_START,
    KEY_INDEX_RIGHT,
    KEY_INDEX_LEFT,
    KEY_INDEX_UP,
    KEY_INDEX_DOWN,
    KEY_INDEX_R,
    KEY_INDEX_L,
    KEY_INDEX_MAX,
};

enum
{
    // Key bits as used by KEYINPUT and KEYCNT.
    KEY_A               = 1 << KEY_INDEX_A,
    KEY_B               = 1 << KEY_INDEX_B,
    KEY_SELECT          = 1 << KEY_INDEX_SELECT,
    KEY_START           = 1 << KEY_INDEX_START,
    KEY_RIGHT           = 1 << KEY_INDEX_RIGHT,
    KEY_LEFT            = 1 << KEY_INDEX_LEFT,
    KEY_UP              = 1 << KEY_INDEX_UP,
    KEY_DOWN            = 1 << KEY_INDEX_DOWN,
    KEY_R               = 1 << KEY_INDEX_R,
    KEY_L               = 1 << KEY_INDEX_L,
};

enum
{
    // Groupings of key bits.
    KEYS_DPAD_X         = KEY_LEFT      | KEY_RIGHT,
    KEYS_DPAD_Y         = KEY_UP        | KEY_DOWN,
    KEYS_DPAD           = KEYS_DPAD_X   | KEYS_DPAD_Y,
    KEYS_AB             = KEY_A         | KEY_B,
    KEYS_LR             = KEY_L         | KEY_R,
    KEYS_STARTSELECT    = KEY_START     | KEY_SELECT,
    KEYS_BUTTONS        = KEYS_AB       | KEYS_LR       | KEYS_STARTSELECT,
    KEYS_ALL            = KEYS_DPAD     | KEYS_BUTTONS,
};

enum
{
    KEY_IRQ_ENABLE       = 1 << 14,
    KEY_IRQ_PRESS_ALL    = 1 << 15,
    KEY_IRQ_PRESS_ANY    = !KEY_IRQ_PRESS_ALL,
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

#define DmaSet(dma_num, src, dest, control)                        \
{                                                                  \
    u32 volatile * regs = (u32 volatile *) &REG_DMA##dma_num##SRC; \
    regs[0] = (u32 volatile)(src);                                 \
    regs[1] = (u32 volatile)(dest);                                \
    regs[2] = (u32 volatile)(control);                             \
    regs[2];                                                       \
}

#define DMA_FILL(dma_num, value, dest, size, bit)                                             \
{                                                                                             \
    u##bit volatile tmp = (u##bit volatile)(value);                                           \
    DmaSet(dma_num,                                                                           \
           &tmp,                                                                              \
           dest,                                                                              \
           (DMA_ENABLE | DMA_START_NOW | DMA_##bit##BIT | DMA_SRC_FIXED | DMA_DST_INCREMENT)  \
         | ((size)/(bit/8)));                                                                 \
}

#define DmaFill16(dma_num, value, dest, size) DMA_FILL(dma_num, value, dest, size, 16)
#define DmaFill32(dma_num, value, dest, size) DMA_FILL(dma_num, value, dest, size, 32)

// Note that the DMA clear macros cause the DMA control value to be calculated
// at runtime rather than compile time. The size is divided by the DMA transfer
// unit size (2 or 4 bytes) and then combined with the DMA control flags using a
// bitwise OR operation.

#define DMA_CLEAR(dma_num, dest, size, bit)  \
{                                           \
    u##bit volatile *_dest = (u##bit volatile *)(dest);     \
    u32 _size = size;                       \
    DmaFill##bit(dma_num, 0, _dest, _size);  \
}

#define DmaClear16(dma_num, dest, size) DMA_CLEAR(dma_num, dest, size, 16)
#define DmaClear32(dma_num, dest, size) DMA_CLEAR(dma_num, dest, size, 32)

#define DMA_COPY(dma_num, src, dest, size, bit)                                                  \
    DmaSet(dma_num,                                                                              \
           src,                                                                                  \
           dest,                                                                                 \
           (DMA_ENABLE | DMA_START_NOW | DMA_##bit##BIT | DMA_SRC_INCREMENT | DMA_DST_INCREMENT) \
         | ((size)/(bit/8)))

#define DmaCopy16(dma_num, src, dest, size) DMA_COPY(dma_num, src, dest, size, 16)
#define DmaCopy32(dma_num, src, dest, size) DMA_COPY(dma_num, src, dest, size, 32)

#define DmaStop(dma_num)                                         \
{                                                               \
    vu16 *regs = (vu16 *)REG_ADDR_DMA##dma_num;               \
    regs[5] &= ~(DMA_START_MASK | DMA_DREQ_ON | DMA_REPEAT); \
    regs[5] &= ~DMA_ENABLE;                                  \
    regs[5];                                                 \
}

#define DmaCopyLarge(dma_num, src, dest, size, block, bit) \
{                                                         \
    const void *_src = src;                               \
    void *_dest = (void *)(dest);                         \
    u32 _size = size;                                     \
    while (1)                                             \
    {                                                     \
        DmaCopy##bit(dma_num, _src, _dest, (block));       \
        _src += (block);                                  \
        _dest += (block);                                 \
        _size -= (block);                                 \
        if (_size <= (block))                             \
        {                                                 \
            DmaCopy##bit(dma_num, _src, _dest, _size);     \
            break;                                        \
        }                                                 \
    }                                                     \
}

#define DmaClearLarge(dma_num, dest, size, block, bit) \
{                                                     \
	void *_dest = dest;                               \
    u32 _size = size;                                 \
    while (1)                                         \
    {                                                 \
        DmaFill##bit(dma_num, 0, _dest, (block));      \
        _dest += (block);                             \
        _size -= (block);                             \
        if (_size <= (block))                         \
        {                                             \
            DmaFill##bit(dma_num, 0, _dest, _size);    \
            break;                                    \
        }                                             \
    }                                                 \
}

#define DmaCopyLarge16(dma_num, src, dest, size, block) DmaCopyLarge(dma_num, src, dest, size, block, 16)
#define DmaCopyLarge32(dma_num, src, dest, size, block) DmaCopyLarge(dma_num, src, dest, size, block, 32)

#define DmaClearLarge16(dma_num, dest, size, block) DmaClearLarge(dma_num, dest, size, block, 16)
#define DmaClearLarge32(dma_num, dest, size, block) DmaClearLarge(dma_num, dest, size, block, 32)

#define DmaCopyDefvars(dma_num, src, dest, size, bit) \
{                                                    \
    const void *_src = src;                          \
    void *_dest = (void *)(dest);                    \
    u32 _size = size;                                \
    DmaCopy##bit(dma_num, _src, _dest, _size);        \
}

#define DmaCopy16Defvars(dma_num, src, dest, size) DmaCopyDefvars(dma_num, src, dest, size, 16)
#define DmaCopy32Defvars(dma_num, src, dest, size) DmaCopyDefvars(dma_num, src, dest, size, 32)

#define DmaFillLarge(dma_num, fillval, dest, size, block, bit) \
{                                                             \
    void *_dest = (void *)(dest);                             \
    u32 _size = (u32)(size);                                  \
    while (1)                                                 \
    {                                                         \
        DmaFill##bit(dma_num, fillval, _dest, (block));        \
        _dest += (block);                                     \
        _size -= (block);                                     \
        if (_size <= (block))                                 \
        {                                                     \
            DmaFill##bit(dma_num, fillval, _dest, _size);      \
            break;                                            \
        }                                                     \
    }                                                         \
}

#define DmaFill16Large(dma_num, fillval, dest, size, block) DmaFillLarge(dma_num, fillval, dest, size, block, 16)
#define DmaFill32Large(dma_num, fillval, dest, size, block) DmaFillLarge(dma_num, fillval, dest, size, block, 32)

#define DmaFillDefvars(dma_num, fillval, dest, size, bit) \
{                                                        \
    void *_dest = (void *)(dest);                        \
    u32 _size = (u##bit)(size);                          \
    DmaFill##bit(dma_num, fillval, _dest, _size);         \
}

#define DmaFill16Defvars(dma_num, fillval, dest, size) DmaFillDefvars(dma_num, fillval, dest, size, 16)
#define DmaFill32Defvars(dma_num, fillval, dest, size) DmaFillDefvars(dma_num, fillval, dest, size, 32)

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

// memory

// External Work RAM (256K)
#define MEM_EWRAM               ((void *) 0x02000000)
#define MEM_EWRAM_SIZE          ((usize) 0x40000)

// Internal Work RAM (32K)
#define MEM_IWRAM               ((void *) 0x03000000)
#define MEM_IWRAM_SIZE          ((usize) 0x8000)

// Video Palette (1K)
#define MEM_PALETTE             ((void *) 0x05000000)
#define MEM_PALETTE_SIZE        ((usize) 0x400)

// Video RAM (96K)
#define MEM_VRAM                ((void *) 0x06000000)
#define MEM_VRAM_SIZE           ((usize) 0x18000)

// Object Attribute Memory (1K)
#define MEM_OAM                 ((void*) 0x07000000)
#define MEM_OAM_SIZE            ((usize) 0x400)

// Cartridge ROM (32M)
#define MEM_ROM                 ((void const *) 0x08000000)
#define MEM_ROM_SIZE            ((usize) 0x02000000)

// Cartridge Static RAM (32K)
#define MEM_SRAM                ((void *) 0x0E000000)
#define MEM_SRAM_SIZE           ((usize) 0x8000)
