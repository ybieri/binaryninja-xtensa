#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 4.4.10
    Title: Boolean Option

    BF/BT instructions branch based on boolean register value.
    Encoding (op0=6, t=7): BF uses r=0, BT uses r=1
    LOOP uses r=8,9,A - no conflict with BF/BT
*/

LLIL_LIFTER(opt_boolean)
{
    switch (insn.id)
    {
    case XTENSA_INS_BT:
    {
        if (insn.operand_count != 2 ||
            insn.operands[0].type != XTENSA_OP_BREG ||
            insn.operands[1].type != XTENSA_OP_BRANCH_TARGET)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t flag = FLAG_B0 + insn.operands[0].reg;
        uint64_t target = insn.operands[1].target;

        // if (flag) goto target
        LowLevelILLabel trueLabel, falseLabel;
        il.AddInstruction(il.If(il.Flag(flag), trueLabel, falseLabel));
        il.MarkLabel(trueLabel);
        il.AddInstruction(il.Jump(il.ConstPointer(4, target)));
        il.MarkLabel(falseLabel);
        return true;
    }

    case XTENSA_INS_BF:
    {
        if (insn.operand_count != 2 ||
            insn.operands[0].type != XTENSA_OP_BREG ||
            insn.operands[1].type != XTENSA_OP_BRANCH_TARGET)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t flag = FLAG_B0 + insn.operands[0].reg;
        uint64_t target = insn.operands[1].target;

        // if (!flag) goto target
        LowLevelILLabel trueLabel, falseLabel;
        il.AddInstruction(il.If(il.Not(0, il.Flag(flag)), trueLabel, falseLabel));
        il.MarkLabel(trueLabel);
        il.AddInstruction(il.Jump(il.ConstPointer(4, target)));
        il.MarkLabel(falseLabel);
        return true;
    }

    default:
        break;
    }

    return false;
}