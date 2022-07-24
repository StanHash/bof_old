#pragma once

#define STRUCT_PAD(start, end) u8 pad_##start##_##end[(end) - (start)]

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define QN(fval, n) ((int) ((fval) * (1 << (n))))
#define QN_INT(qval, n) ((int) ((qval) >> (n)))

#define Q4(fval) QN((fval), 4)
#define Q4_INT(qval) QN_INT((qval), 4)

#define Q8(fval) QN((fval), 8)
#define Q8_INT(qval) QN_INT((qval), 8)
