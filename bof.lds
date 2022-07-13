OUTPUT_ARCH(arm)

_M4aNumMusicPlayers = 9;
_M4aMaxLines = 0;

ENTRY(__start)

MEMORY
{
    EWRAM (w!x) : ORIGIN = 0x02000000, LENGTH = 256K
    IWRAM (w!x) : ORIGIN = 0x03000000, LENGTH = 32K
    ROM   (rx)  : ORIGIN = 0x08000000, LENGTH = 32M
}

SECTIONS
{
    ewram (NOLOAD) : ALIGN(4)
    {
        . = 0x03E000; src/m4a.o(.SoundInfo);
        . = 0x03EFB0; src/m4a.o(.MPlayJumpTable);
        . = 0x03F040; src/m4a.o(.CgbChans);
        . = 0x03FE40; src/m4a.o(.MPlayMemAccArea);

        . = 0x040000; /* end */
    } > EWRAM

    iwram (NOLOAD) : ALIGN(4)
    {
        /* .bss */
        . = 0x001E78;
        . = ALIGN(4); *libc.a:syscalls.o(.bss);
        . = ALIGN(4); *libgcc.a:dp-bit.o(.bss);
        . = ALIGN(4); *libgcc.a:fp-bit.o(.bss);
        . = 0x001F50; src/m4a.o(.bss);

        /* COMMON? */

        . = 0x002460; IntrTable = .;

        . = 0x006ED0; *libc.a:sbrkr.o(COMMON);
        . = ALIGN(4); end = .;

        . = 0x008000; /* end */
    } > IWRAM

    rom : ALIGN(4)
    {
        /* .text */
        src/crt0.o(.text);
        asm/code.o(.text);
        src/m4a_1.o(.text);
        src/m4a.o(.text);
        src/agbsyscalls.o(.text);
        src/agbprint.o(.text);
        src/agbsram.o(.text);
        *libgcc.a:_call_via_rX.o(.text);
        *libgcc.a:_divsi3.o(.text);
        *libgcc.a:_dvmd_tls.o(.text);
        *libgcc.a:_modsi3.o(.text);
        *libgcc.a:_udivsi3.o(.text);
        *libgcc.a:_umodsi3.o(.text);
        *libc.a:bzero.o(.text);
        *libc.a:memcpy.o(.text);
        *libc.a:memset.o(.text);
        *libc.a:vsprintf.o(.text);
        *libc.a:vfprintf.o(.text);
        *libc.a:wsetup.o(.text);
        *libc.a:dtoa.o(.text);
        *libc.a:fflush.o(.text);
        *libc.a:findfp.o(.text);
        *libc.a:freer.o(.text);
        *libc.a:fvwrite.o(.text);
        *libc.a:fwalk.o(.text);
        *libc.a:locale.o(.text);
        *libc.a:makebuf.o(.text);
        *libc.a:mallocr.o(.text);
        *libc.a:mbtowc_r.o(.text);
        *libc.a:memchr.o(.text);
        *libc.a:memmove.o(.text);
        *libc.a:mlock.o(.text);
        *libc.a:mprec.o(.text);
        *libc.a:s_isinf.o(.text);
        *libc.a:s_isnan.o(.text);
        *libc.a:sbrkr.o(.text);
        *libc.a:stdio.o(.text);
        *libc.a:strcmp.o(.text);
        *libc.a:strlen.o(.text);
        *libc.a:syscalls.o(.text);
        *libc.a:writer.o(.text);
        *libc.a:callocr.o(.text);
        *libc.a:closer.o(.text);
        *libc.a:errno.o(.text);
        *libc.a:fstatr.o(.text);
        *libc.a:libcfunc.o(.text);
        *libc.a:lseekr.o(.text);
        *libc.a:readr.o(.text);
        *libgcc.a:dp-bit.o(.text);
        *libgcc.a:fp-bit.o(.text);
        *libgcc.a:_lshrdi3.o(.text);
        *libgcc.a:_muldi3.o(.text);
        *libgcc.a:_negdi2.o(.text);

        /* .rodata */
        . = ALIGN(4); asm/rodata.o(.rodata);
        . = ALIGN(4); src/m4a.o(.rodata);
        . = ALIGN(4); asm/rodata_sound.o(.rodata);
        . = ALIGN(4); src/agbprint.o(.rodata);
        . = ALIGN(4); src/agbsram.o(.rodata);
        . = ALIGN(4); *libc.a:impure.o(.rodata);
        . = ALIGN(4); *libc.a:vfprintf.o(.rodata);
        . = ALIGN(4); *libc.a:dtoa.o(.rodata);
        . = ALIGN(4); *libc.a:locale.o(.rodata);
        . = ALIGN(4); *libc.a:mprec.o(.rodata);
        . = ALIGN(4); *libc.a:syscalls.o(.rodata);

        . = ALIGN(0x200);

        /* .data */
        . = ALIGN(4); asm/data.o(.data);
        . = ALIGN(4); *libc.a:impure.o(.data);
        . = ALIGN(4); *libc.a:locale.o(.data);
        . = ALIGN(4); *libc.a:mallocr.o(.data);

        . = 0x00400000; /* end */
    } > ROM = 0

    /* DWARF 2 sections */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    .debug_info     0 : { *(.debug_info) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }

    /* Discard everything not specifically mentioned above. */
    /DISCARD/ :
    {
        *(*);
    }
}
