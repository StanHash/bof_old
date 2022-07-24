#include "types.h"
#include "agbio.h"
#include "unknown.h"
#include "m4a.h"

void AgbMain(void)
{
    int dummy;

    // TODO: MEM constants
    DmaFill32(3, 0, 0x02000000, 0x00040000);
    DmaFill32(3, 0, 0x03000000, 0x00007E00);

    func_08000274(&Unk_03002410);
    func_08000570(func_0800198C, &Unk_03002410, CONSTANT_UNK_03002410_SLOT_AUTO);
    func_08000570(func_08002500, &Unk_03002410, 3);
    IntrInit();
    m4aSoundInit();
    func_080004B0(&Unk_03002410);
}
