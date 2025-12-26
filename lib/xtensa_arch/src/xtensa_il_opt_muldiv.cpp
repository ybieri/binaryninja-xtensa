#include "lowlevelilinstruction.h"
#include "xtensa_arch.h"
#include "xtensa_il.h"


LLIL_LIFTER(opt_muldiv)
{
    switch (insn.id)
    {
    case XTENSA_INS_REMU:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("REMU instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Unimplemented());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("REMU instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Unimplemented());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.ModUnsigned(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }

    case XTENSA_INS_QUOU:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("QUOU instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("QUOU instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.DivUnsigned(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }

    case XTENSA_INS_QUOS:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("QUOS instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("QUOS instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.DivSigned(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }

    case XTENSA_INS_REMS:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("REMS instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("REMS instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.ModSigned(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }

    case XTENSA_INS_MULL:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("MULL instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("MULL instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.Mult(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }

    case XTENSA_INS_MULSH:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("MULSH instruction at 0x%" PRIx64 " has unexpected operand cound: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }

        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("MULSH instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.MultDoublePrecSigned(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }



    case XTENSA_INS_MULUH:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("MULUH instruction at 0x%" PRIx64 " has unexpected operand cound: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }

        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("MULUH instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.MultDoublePrecUnsigned(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }

    default:
        break;
    }

    return false;
}