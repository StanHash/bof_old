#pragma once

#define ALIGN(n)      __attribute__((aligned(n)))
#define SECTION(name) __attribute__((section(name)))
#define PACKED        __attribute__((packed))

#define CONST_DATA SECTION(".data")
#define EWRAM_DATA SECTION(".ewram")
#define COMMON(name) SECTION("."#name)
#define SHOULD_BE_CONST
#define SHOULD_BE_STATIC
