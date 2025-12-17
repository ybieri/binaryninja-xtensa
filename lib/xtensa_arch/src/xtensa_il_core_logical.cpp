#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 3.8.8
    Title: Bitwise Logical Instructions
*/

LLIL_LIFTER(core_logical)
{
    switch (insn.id)
    {
    case XTENSA_INS_AND:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("AND instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("AND instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = insn.operands[0].reg;
        uint32_t src1 = insn.operands[1].reg;
        uint32_t src2 = insn.operands[2].reg;

        il.AddInstruction(il.SetRegister(4, dest, il.And(4, il.Register(4, src1), il.Register(4, src2))));

        return true;
    }

    case XTENSA_INS_OR:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("OR instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("OR instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = insn.operands[0].reg;
        uint32_t src1 = insn.operands[1].reg;
        uint32_t src2 = insn.operands[2].reg;

        il.AddInstruction(il.SetRegister(4, dest, il.Or(4, il.Register(4, src1), il.Register(4, src2))));

        return true;
    }

    case XTENSA_INS_XOR:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("XOR instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("XOR instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = insn.operands[0].reg;
        uint32_t src1 = insn.operands[1].reg;
        uint32_t src2 = insn.operands[2].reg;

        il.AddInstruction(il.SetRegister(4, dest, il.Xor(4, il.Register(4, src1), il.Register(4, src2))));

        return true;
    }

    default:
        break;
    }
    return false;
}
