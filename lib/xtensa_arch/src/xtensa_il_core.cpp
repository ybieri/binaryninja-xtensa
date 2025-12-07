#include "xtensa_il.h"
#include "xtensa_arch.h"

LLIL_LIFTER(core)
{
    switch (insn.id)
    {
        // TODO: XTENSA_INS_WER not implemented in decoder
        // case XTENSA_INS_WER:
        // { ... }

        case XTENSA_INS_ILL:
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        default: break;
    }

    return false;
}