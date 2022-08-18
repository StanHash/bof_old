#include "types.h"
#include "agbio.h"
#include "unknown.h"
#include "m4a.h"

void AgbMain(void)
{
    u32 dummy;

    DmaFill32(3, 0, MEM_EWRAM, MEM_EWRAM_SIZE);
    DmaFill32(3, 0, MEM_IWRAM, MEM_IWRAM_SIZE - 0x200);

    func_08000274(&Unk_03002410);
    func_08000570(func_0800198C, &Unk_03002410, CONSTANT_UNK_03002410_SLOT_AUTO);
    func_08000570(func_08002500, &Unk_03002410, 3);
    IntrInit();
    m4aSoundInit();
    func_080004B0(&Unk_03002410);
}
