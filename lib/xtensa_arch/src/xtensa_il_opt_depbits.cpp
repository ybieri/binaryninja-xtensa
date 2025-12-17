#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 4.3.9
    Title: Deposit Bits Option
*/

LLIL_LIFTER(opt_depbits)
{
    switch (insn.id)
    {
    case XTENSA_INS_DEPBITS:
    {
        // TODO: need implementation
        il.AddInstruction(il.Unimplemented());
        return true;
    }

    default:
        break;
    }

    return false;
}