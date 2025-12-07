#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 3.8.4
    Title: Jump and Call Instructions
    Note: This code only include core call/ret insturctions
*/

LLIL_LIFTER(core_call)
{
    switch (insn.id)
    {
        case XTENSA_INS_CALL0:
        {
            if (insn.operand_count != 1)
            {
                LogWarn("CALL0 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("CALL0 instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint64_t target_addr = insn.operands[0].target;

            il.AddInstruction(il.Call(
                il.Const(4, target_addr)
            ));

            return true;
        }

        case XTENSA_INS_CALLX0:
        {
            if (insn.operand_count != 1)
            {
                LogWarn("CALLX0 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("CALLX0 instruction at 0x%" PRIx64 " has non-register operand", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t src = insn.operands[0].reg;

            il.AddInstruction(il.Call(
                il.Register(4, src)
            ));

            return true;
        }

        case XTENSA_INS_RET:
        {
            il.AddInstruction(il.Return(
                il.Register(4, REG_A0)
            ));
            return true;
        }

        default: break;
    }

    return false;
}
