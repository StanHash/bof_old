#pragma once

#include "types.h"

void AgbPrintInit(void);
void AgbPutc(char chr);
void AgbPrint(char const * str);
void AgbPrintf(char const * fmt, ...);
void AgbPrintFlush1Block(void);
void AgbPrintFlush(void);
void AgbAssert(char const * file, int line, char const * expr, int is_fatal);
