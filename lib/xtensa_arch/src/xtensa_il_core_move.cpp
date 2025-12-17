#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 3.8.6
    Title: Move Instructions
*/

LLIL_LIFTER(core_move)
{
    switch (insn.id)
    {
    case XTENSA_INS_MOVI:
    {
        if (insn.operand_count != 2)
        {
            LogWarn("MOVI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_IMM)
        {
            LogWarn("MOVI instruction at 0x%" PRIx64 " has unexpected operand types", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = insn.operands[0].reg;
        int32_t imm = insn.operands[1].imm;

        il.AddInstruction(il.SetRegister(4, dest, il.Const(4, imm)));

        return true;
    }

    case XTENSA_INS_MOVEQZ:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("MOVEQZ instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("MOVEQZ instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = insn.operands[0].reg;  // ar
        uint32_t src = insn.operands[1].reg;   // as
        uint32_t cond = insn.operands[2].reg;  // at (tested for zero)

        // Conditional move: if (AR[at] == 0) AR[ar] = AR[as]
        LowLevelILLabel trueLabel, falseLabel, doneLabel;

        // Test condition register for zero
        il.AddInstruction(il.If(il.CompareEqual(4, il.Register(4, cond), il.Const(4, 0)), trueLabel, falseLabel));

        // True branch: dest = src
        il.MarkLabel(trueLabel);
        il.AddInstruction(il.SetRegister(4, dest, il.Register(4, src)));
        il.AddInstruction(il.Goto(doneLabel));

        // False branch: dest unchanged (no operation)
        il.MarkLabel(falseLabel);
        // No operation - destination register remains unchanged

        // Done label
        il.MarkLabel(doneLabel);

        return true;
    }

    case XTENSA_INS_MOVNEZ:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("MOVNEZ instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("MOVNEZ instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = insn.operands[0].reg;  // ar
        uint32_t src = insn.operands[1].reg;   // as
        uint32_t cond = insn.operands[2].reg;  // at (tested for non-zero)

        // Conditional move: if (AR[at] != 0) AR[ar] = AR[as]
        LowLevelILLabel trueLabel, falseLabel, doneLabel;

        // Test condition register for non-zero
        il.AddInstruction(il.If(il.CompareNotEqual(4, il.Register(4, cond), il.Const(4, 0)), trueLabel, falseLabel));

        // True branch: dest = src
        il.MarkLabel(trueLabel);
        il.AddInstruction(il.SetRegister(4, dest, il.Register(4, src)));
        il.AddInstruction(il.Goto(doneLabel));

        // False branch: dest unchanged (no operation)
        il.MarkLabel(falseLabel);
        // No operation - destination register remains unchanged

        // Done label
        il.MarkLabel(doneLabel);

        return true;
    }

    case XTENSA_INS_MOVLTZ:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("MOVLTZ instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("MOVLTZ instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = insn.operands[0].reg;  // ar
        uint32_t src = insn.operands[1].reg;   // as
        uint32_t cond = insn.operands[2].reg;  // at (tested for < 0)

        // Conditional move: if (AR[at] < 0) AR[ar] = AR[as]
        LowLevelILLabel trueLabel, falseLabel, doneLabel;

        // Test condition register for less than zero (signed)
        il.AddInstruction(
            il.If(il.CompareSignedLessThan(4, il.Register(4, cond), il.Const(4, 0)), trueLabel, falseLabel));

        // True branch: dest = src
        il.MarkLabel(trueLabel);
        il.AddInstruction(il.SetRegister(4, dest, il.Register(4, src)));
        il.AddInstruction(il.Goto(doneLabel));

        // False branch: dest unchanged
        il.MarkLabel(falseLabel);

        il.MarkLabel(doneLabel);
        return true;
    }

    case XTENSA_INS_MOVGEZ:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("MOVGEZ instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("MOVGEZ instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = insn.operands[0].reg;  // ar
        uint32_t src = insn.operands[1].reg;   // as
        uint32_t cond = insn.operands[2].reg;  // at (tested for >= 0)

        // Conditional move: if (AR[at] >= 0) AR[ar] = AR[as]
        LowLevelILLabel trueLabel, falseLabel, doneLabel;

        // Test condition register for greater or equal to zero (signed)
        il.AddInstruction(
            il.If(il.CompareSignedGreaterEqual(4, il.Register(4, cond), il.Const(4, 0)), trueLabel, falseLabel));

        // True branch: dest = src
        il.MarkLabel(trueLabel);
        il.AddInstruction(il.SetRegister(4, dest, il.Register(4, src)));
        il.AddInstruction(il.Goto(doneLabel));

        // False branch: dest unchanged
        il.MarkLabel(falseLabel);

        il.MarkLabel(doneLabel);
        return true;
    }

    default:
        break;
    }

    return false;
}
