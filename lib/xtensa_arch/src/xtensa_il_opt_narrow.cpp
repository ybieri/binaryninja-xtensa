#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 4.3.1
    Title: Code Density Option
*/

LLIL_LIFTER(opt_narrow)
{
    switch (insn.id)
    {
    case XTENSA_INS_ADD_N:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("ADD.N instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_REG
            || insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("ADD.N instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = insn.operands[0].reg;
        uint32_t src1 = insn.operands[1].reg;
        uint32_t src2 = insn.operands[2].reg;

        il.AddInstruction(il.SetRegister(4, dest, il.Add(4, il.Register(4, src1), il.Register(4, src2))));

        return true;
    }

    case XTENSA_INS_ADDI_N:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("ADDI.N instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_REG
            || insn.operands[2].type != XTENSA_OP_IMM)
        {
            LogWarn("ADDI.N instruction at 0x%" PRIx64 " has unexpected operand types", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = insn.operands[0].reg;
        uint32_t src = insn.operands[1].reg;
        int32_t imm = insn.operands[2].imm;

        il.AddInstruction(il.SetRegister(4, dest, il.Add(4, il.Register(4, src), il.Const(4, imm))));

        return true;
    }

    case XTENSA_INS_BEQZ_N:
    {
        if (insn.operand_count != 2)
        {
            LogWarn("BEQZ.N instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG)
        {
            LogWarn("BEQZ.N instruction at 0x%" PRIx64 " has non-register operand 0", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[1].type != XTENSA_OP_BRANCH_TARGET)
        {
            LogWarn("BEQZ.N instruction at 0x%" PRIx64 " has unexpected operand type", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t s = insn.operands[0].reg;
        uint64_t target_addr = insn.operands[1].target;

        LowLevelILLabel trueLabel, falseLabel;

        il.AddInstruction(il.If(il.CompareEqual(4, il.Register(4, s), il.Const(4, 0)), trueLabel, falseLabel));

        il.MarkLabel(trueLabel);
        il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

        il.MarkLabel(falseLabel);

        return true;
    }

    case XTENSA_INS_BNEZ_N:
    {
        if (insn.operand_count != 2)
        {
            LogWarn("BNEZ.N instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG)
        {
            LogWarn("BNEZ.N instruction at 0x%" PRIx64 " has non-register operand 0", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[1].type != XTENSA_OP_BRANCH_TARGET)
        {
            LogWarn("BNEZ.N instruction at 0x%" PRIx64 " has unexpected operand type", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t s = insn.operands[0].reg;
        uint64_t target_addr = insn.operands[1].target;

        LowLevelILLabel trueLabel, falseLabel;

        il.AddInstruction(il.If(il.CompareNotEqual(4, il.Register(4, s), il.Const(4, 0)), trueLabel, falseLabel));

        il.MarkLabel(trueLabel);
        il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

        il.MarkLabel(falseLabel);
        return true;
    }

    case XTENSA_INS_BREAK_N:
    {
        il.AddInstruction(il.Breakpoint());
        return true;
    }

    case XTENSA_INS_ILL_N:
    {
        il.AddInstruction(il.Trap(0));
        return true;
    }

    case XTENSA_INS_L32I_N:
    {
        if (insn.operand_count != 2)
        {
            LogWarn("L32I.N instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_MEM)
        {
            LogWarn("L32I.N instruction at 0x%" PRIx64 " has unexpected operand types", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = insn.operands[0].reg;
        uint32_t base = insn.operands[1].mem.base;
        int32_t offset = insn.operands[1].mem.offset;

        il.AddInstruction(il.SetRegister(4, dest, il.Load(4, il.Add(4, il.Register(4, base), il.Const(4, offset)))));
        return true;
    }

    case XTENSA_INS_MOV_N:
    {
        if (insn.operand_count != 2)
        {
            LogWarn("MOV.N instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_REG)
        {
            LogWarn("MOV.N instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = insn.operands[0].reg;
        uint32_t src = insn.operands[1].reg;

        // Simple register-to-register move
        il.AddInstruction(il.SetRegister(4, dest, il.Register(4, src)));
        return true;
    }

    case XTENSA_INS_MOVI_N:
    {
        if (insn.operand_count != 2)
        {
            LogWarn("MOVI.N instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_IMM)
        {
            LogWarn("MOVI.N instruction at 0x%" PRIx64 " has unexpected operand types", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = insn.operands[0].reg;
        int32_t imm = insn.operands[1].imm;

        // Validate immediate range (-32 to 95)
        if (imm < -32 || imm > 95)
        {
            LogWarn("MOVI.N instruction at 0x%" PRIx64 " has out-of-range immediate: %d", addr, imm);
            il.AddInstruction(il.Undefined());
            return true;
        }

        // Simple move of sign-extended immediate to destination register
        il.AddInstruction(il.SetRegister(4, dest, il.Const(4, imm)));

        return true;
    }

    case XTENSA_INS_NOP_N:
    {
        il.AddInstruction(il.Nop());
        return true;
    }

    case XTENSA_INS_RET_N:
    {
        il.AddInstruction(il.Return(il.Register(4, REG_A0)));
        return true;
    }

    case XTENSA_INS_RETW_N:
    {
        il.AddInstruction(il.Return(il.Register(4, REG_A0)));
        return true;
    }

    case XTENSA_INS_S32I_N:
    {
        if (insn.operand_count != 2)
        {
            LogWarn("S32I.N instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_MEM)
        {
            LogWarn("S32I.N instruction at 0x%" PRIx64 " has unexpected operand types", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t src = insn.operands[0].reg;
        uint32_t base = insn.operands[1].mem.base;
        int32_t offset = insn.operands[1].mem.offset;

        il.AddInstruction(il.Store(4, il.Add(4, il.Register(4, base), il.Const(4, offset)), il.Register(4, src)));

        return true;
    }

    default:
        break;
    }

    return false;
}
