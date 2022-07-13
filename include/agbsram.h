#pragma once

#include "types.h"

void ReadSram(u8 const * src, u8 * dst, u32 size);
void WriteSram(u8 const * src, u8 * dst, u32 size);
u32 VerifySram(u8 const * src, u8 * dst, u32 size);
