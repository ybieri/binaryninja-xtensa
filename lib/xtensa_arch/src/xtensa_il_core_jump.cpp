#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 3.8.4
    Title: Jump and Call Instructions
    Note: This code only include core jump instructions
*/

LLIL_LIFTER(core_jump)
{
    switch (insn.id)
    {
        case XTENSA_INS_J:
        {
            if (insn.operand_count != 1)
            {
                LogWarn("J instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("J instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint64_t target_addr = insn.operands[0].target;

            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));
            return true;
        }

        default: break;
    }
    return false;
}
