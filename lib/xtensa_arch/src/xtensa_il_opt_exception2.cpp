#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 4.4.1
    Title: Exception Option 2
*/

LLIL_LIFTER(opt_exception2)
{
    switch (insn.id)
    {
        // TODO: XTENSA_INS_EXCW unimplemented in decoder
        // case XTENSA_INS_EXCW: { ... }

    case XTENSA_INS_SYSCALL:
    {
        // TODO: need implementation
        il.AddInstruction(il.Unimplemented());
        return true;
    }

    case XTENSA_INS_RFE:
    {
        // TODO: need implementation
        il.AddInstruction(il.Unimplemented());
        return true;
    }

        // TODO: XTENSA_INS_RFDE unimplemented in decoder
        // case XTENSA_INS_RFDE: { ... }

    default:
        break;
    }

    return false;
}