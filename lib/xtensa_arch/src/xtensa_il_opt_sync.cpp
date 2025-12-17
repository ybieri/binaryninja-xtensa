#include "xtensa_il.h"
#include "xtensa_arch.h"

LLIL_LIFTER(opt_sync)
{
    switch (insn.id)
    {
    case XTENSA_INS_L32AI:
    {
        // TODO: need implementation
        il.AddInstruction(il.Unimplemented());
        return true;
    }

    case XTENSA_INS_S32RI:
    {
        // TODO: need implementation
        il.AddInstruction(il.Unimplemented());
        return false;
    }

    default:
        break;
    }

    return false;
}