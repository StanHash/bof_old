#pragma once

#include <stddef.h>
#include <stdint.h>
#include <limits.h>

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef ptrdiff_t isize;
typedef intptr_t  iptr;

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef size_t    usize;
typedef uintptr_t uptr;

typedef u8  fu8;
typedef u16 fu16;
typedef i8  fi8;
typedef i16 fi16;

typedef u32 uint;

enum Bool { FALSE, TRUE };
