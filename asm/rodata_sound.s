    .section .rodata

    .incbin "baserom.gba", 0x142684, 0x142B5C - 0x142684

    .global MusicPlayerTable
MusicPlayerTable: @ 08142B5C

    .incbin "baserom.gba", 0x142B5C, 0x142BC8 - 0x142B5C

    .global SongTable
SongTable: @ 08142BC8

    .incbin "baserom.gba", 0x142BC8, 0x17AF80 - 0x142BC8
