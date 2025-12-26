#include "xtensa_arch.h"
#include "xtensa_il.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 3.8.7
    Title: Arithmetic Instructions
*/

LLIL_LIFTER(core_arithmetic)
{
    switch (insn.id)
    {
    case XTENSA_INS_ADD:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("ADD instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("ADD instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.Add(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }

    case XTENSA_INS_ADDX2:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("ADDX2 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("ADDX2 instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.Add(4,
                    il.ShiftLeft(4,
                        il.Register(4, src1),
                        il.Const(4, 1)
                    ),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }

    case XTENSA_INS_ADDX4:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("ADDX4 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("ADDX4 instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.Add(4,
                    il.ShiftLeft(4,
                        il.Register(4, src1),
                        il.Const(4, 2)
                    ),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }

    case XTENSA_INS_ADDX8:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("ADDX8 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("ADDX8 instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.Add(4,
                    il.ShiftLeft(4,
                        il.Register(4, src1),
                        il.Const(4, 3)
                    ),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }

    case XTENSA_INS_SUB:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("SUB instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("SUB instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.Sub(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }

    case XTENSA_INS_SUBX2:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("SUBX2 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("SUBX2 instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.Sub(4,
                    il.ShiftLeft(4,
                        il.Register(4, src1),
                        il.Const(4, 1)
                    ),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }

    case XTENSA_INS_SUBX4:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("SUBX4 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("SUBX4 instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.Sub(4,
                    il.ShiftLeft(4,
                        il.Register(4, src1),
                        il.Const(4, 2)
                    ),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }

    case XTENSA_INS_SUBX8:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("SUBX8 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("SUBX8 instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        uint32_t src2 = REG_A0 + insn.operands[2].reg;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.Sub(4,
                    il.ShiftLeft(4,
                        il.Register(4, src1),
                        il.Const(4, 3)
                    ),
                    il.Register(4, src2)
                )
            )
        );

        return true;
    }

    case XTENSA_INS_NEG:
    {
        if (insn.operand_count != 2)
        {
            LogWarn("NEG instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_REG)
        {
            LogWarn("NEG instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;

        il.AddInstruction(il.SetRegister(4, dest, il.Neg(4, il.Register(4, src1))));

        return true;
    }

    case XTENSA_INS_ABS:
    {
        if (insn.operand_count != 2)
        {
            LogWarn("ABS instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_REG)
        {
            LogWarn("ABS instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;

        LowLevelILLabel trueLabel, falseLabel, doneLabel;

        il.AddInstruction(
            il.If(
                il.CompareSignedLessThan(4,
                    il.Register(4, src1),
                    il.Const(4, 0)
                ),
                trueLabel, falseLabel
            )
        );

        il.MarkLabel(trueLabel);
        il.AddInstruction(il.SetRegister(4, dest, il.Neg(4, il.Register(4, src1))));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(falseLabel);
        il.AddInstruction(il.SetRegister(4, dest, il.Register(4, src1)));

        il.MarkLabel(doneLabel);

        return true;
    }

    case XTENSA_INS_ADDI:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("ADDI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_IMM)
        {
            LogWarn("ADDI instruction at 0x%" PRIx64 " has unexpected operand types", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        int32_t imm = insn.operands[2].imm;

        il.AddInstruction(il.SetRegister(4, dest, il.Add(4, il.Register(4, src1), il.Const(4, imm))));

        return true;
    }

    case XTENSA_INS_ADDMI:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("ADDMI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_IMM)
        {
            LogWarn("ADDMI instruction at 0x%" PRIx64 " has unexpected operand types", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src1 = REG_A0 + insn.operands[1].reg;
        int32_t imm = insn.operands[2].imm;

        il.AddInstruction(il.SetRegister(4, dest, il.Add(4, il.Register(4, src1), il.Const(4, imm))));

        return true;
    }

    // TODO: SALT and SALTU is unimplemented in decoder.

    default:
        break;
    }

    return false;
}
