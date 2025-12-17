#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 4.3.9
    Title: Floating-Point Coprocessor Option (Single Precision)
*/

// Helper to convert FP operand register number (0-15) to architecture register ID
static inline uint32_t fp_reg(const XtensaOperand& op)
{
    return REG_F0 + op.reg;
}

LLIL_LIFTER(opt_fp)
{
    switch (insn.id)
    {
    case XTENSA_INS_ADD_S:
    {
        if (insn.operand_count != 3 || insn.operands[0].type != XTENSA_OP_FPREG
            || insn.operands[1].type != XTENSA_OP_FPREG || insn.operands[2].type != XTENSA_OP_FPREG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = fp_reg(insn.operands[0]);
        uint32_t src1 = fp_reg(insn.operands[1]);
        uint32_t src2 = fp_reg(insn.operands[2]);

        il.AddInstruction(il.SetRegister(4, dest, il.FloatAdd(4, il.Register(4, src1), il.Register(4, src2))));
        return true;
    }

    case XTENSA_INS_SUB_S:
    {
        if (insn.operand_count != 3 || insn.operands[0].type != XTENSA_OP_FPREG
            || insn.operands[1].type != XTENSA_OP_FPREG || insn.operands[2].type != XTENSA_OP_FPREG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = fp_reg(insn.operands[0]);
        uint32_t src1 = fp_reg(insn.operands[1]);
        uint32_t src2 = fp_reg(insn.operands[2]);

        il.AddInstruction(il.SetRegister(4, dest, il.FloatSub(4, il.Register(4, src1), il.Register(4, src2))));
        return true;
    }

    case XTENSA_INS_MUL_S:
    {
        if (insn.operand_count != 3 || insn.operands[0].type != XTENSA_OP_FPREG
            || insn.operands[1].type != XTENSA_OP_FPREG || insn.operands[2].type != XTENSA_OP_FPREG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = fp_reg(insn.operands[0]);
        uint32_t src1 = fp_reg(insn.operands[1]);
        uint32_t src2 = fp_reg(insn.operands[2]);

        il.AddInstruction(il.SetRegister(4, dest, il.FloatMult(4, il.Register(4, src1), il.Register(4, src2))));
        return true;
    }

    case XTENSA_INS_MOV_S:
    {
        if (insn.operand_count != 2 || insn.operands[0].type != XTENSA_OP_FPREG
            || insn.operands[1].type != XTENSA_OP_FPREG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = fp_reg(insn.operands[0]);
        uint32_t src = fp_reg(insn.operands[1]);

        il.AddInstruction(il.SetRegister(4, dest, il.Register(4, src)));
        return true;
    }

    case XTENSA_INS_ABS_S:
    {
        if (insn.operand_count != 2 || insn.operands[0].type != XTENSA_OP_FPREG
            || insn.operands[1].type != XTENSA_OP_FPREG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = fp_reg(insn.operands[0]);
        uint32_t src = fp_reg(insn.operands[1]);

        il.AddInstruction(il.SetRegister(4, dest, il.FloatAbs(4, il.Register(4, src))));
        return true;
    }

    case XTENSA_INS_NEG_S:
    {
        if (insn.operand_count != 2 || insn.operands[0].type != XTENSA_OP_FPREG
            || insn.operands[1].type != XTENSA_OP_FPREG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = fp_reg(insn.operands[0]);
        uint32_t src = fp_reg(insn.operands[1]);

        il.AddInstruction(il.SetRegister(4, dest, il.FloatNeg(4, il.Register(4, src))));
        return true;
    }

    case XTENSA_INS_LSI:
    {
        if (insn.operand_count != 2 || insn.operands[0].type != XTENSA_OP_FPREG
            || insn.operands[1].type != XTENSA_OP_MEM)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = fp_reg(insn.operands[0]);
        uint32_t base = insn.operands[1].mem.base;
        int32_t offset = insn.operands[1].mem.offset;

        il.AddInstruction(il.SetRegister(4, dest, il.Load(4, il.Add(4, il.Register(4, base), il.Const(4, offset)))));
        return true;
    }

    case XTENSA_INS_SSI:
    {
        if (insn.operand_count != 2 || insn.operands[0].type != XTENSA_OP_FPREG
            || insn.operands[1].type != XTENSA_OP_MEM)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t src = fp_reg(insn.operands[0]);
        uint32_t base = insn.operands[1].mem.base;
        int32_t offset = insn.operands[1].mem.offset;

        il.AddInstruction(il.Store(4, il.Add(4, il.Register(4, base), il.Const(4, offset)), il.Register(4, src)));
        return true;
    }

    case XTENSA_INS_LSIU:
    {
        if (insn.operand_count != 2 || insn.operands[0].type != XTENSA_OP_FPREG
            || insn.operands[1].type != XTENSA_OP_MEM)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = fp_reg(insn.operands[0]);
        uint32_t base = insn.operands[1].mem.base;
        int32_t offset = insn.operands[1].mem.offset;

        // Calculate new address
        auto addr_expr = il.Add(4, il.Register(4, base), il.Const(4, offset));

        // Load value into FR
        il.AddInstruction(il.SetRegister(4, dest, il.Load(4, addr_expr)));

        // Update base register
        il.AddInstruction(il.SetRegister(4, base, il.Add(4, il.Register(4, base), il.Const(4, offset))));
        return true;
    }

    case XTENSA_INS_SSIU:
    {
        if (insn.operand_count != 2 || insn.operands[0].type != XTENSA_OP_FPREG
            || insn.operands[1].type != XTENSA_OP_MEM)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t src = fp_reg(insn.operands[0]);
        uint32_t base = insn.operands[1].mem.base;
        int32_t offset = insn.operands[1].mem.offset;

        // Calculate new address
        auto addr_expr = il.Add(4, il.Register(4, base), il.Const(4, offset));

        // Store value from FR
        il.AddInstruction(il.Store(4, addr_expr, il.Register(4, src)));

        // Update base register
        il.AddInstruction(il.SetRegister(4, base, il.Add(4, il.Register(4, base), il.Const(4, offset))));
        return true;
    }

    case XTENSA_INS_LSX:
    {
        if (insn.operand_count != 3 || insn.operands[0].type != XTENSA_OP_FPREG
            || insn.operands[1].type != XTENSA_OP_REG || insn.operands[2].type != XTENSA_OP_REG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = fp_reg(insn.operands[0]);
        uint32_t base = insn.operands[1].reg;
        uint32_t index = insn.operands[2].reg;

        il.AddInstruction(il.SetRegister(4, dest, il.Load(4, il.Add(4, il.Register(4, base), il.Register(4, index)))));
        return true;
    }

    case XTENSA_INS_SSX:
    {
        if (insn.operand_count != 3 || insn.operands[0].type != XTENSA_OP_FPREG
            || insn.operands[1].type != XTENSA_OP_REG || insn.operands[2].type != XTENSA_OP_REG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t src = fp_reg(insn.operands[0]);
        uint32_t base = insn.operands[1].reg;
        uint32_t index = insn.operands[2].reg;

        il.AddInstruction(il.Store(4, il.Add(4, il.Register(4, base), il.Register(4, index)), il.Register(4, src)));
        return true;
    }

    case XTENSA_INS_LSXU:
    {
        if (insn.operand_count != 3 || insn.operands[0].type != XTENSA_OP_FPREG
            || insn.operands[1].type != XTENSA_OP_REG || insn.operands[2].type != XTENSA_OP_REG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = fp_reg(insn.operands[0]);
        uint32_t base = insn.operands[1].reg;
        uint32_t index = insn.operands[2].reg;

        // Load value into FR
        il.AddInstruction(il.SetRegister(4, dest, il.Load(4, il.Add(4, il.Register(4, base), il.Register(4, index)))));

        // Update base register
        il.AddInstruction(il.SetRegister(4, base, il.Add(4, il.Register(4, base), il.Register(4, index))));
        return true;
    }

    case XTENSA_INS_SSXU:
    {
        if (insn.operand_count != 3 || insn.operands[0].type != XTENSA_OP_FPREG
            || insn.operands[1].type != XTENSA_OP_REG || insn.operands[2].type != XTENSA_OP_REG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t src = fp_reg(insn.operands[0]);
        uint32_t base = insn.operands[1].reg;
        uint32_t index = insn.operands[2].reg;

        // Store value from FR
        il.AddInstruction(il.Store(4, il.Add(4, il.Register(4, base), il.Register(4, index)), il.Register(4, src)));

        // Update base register
        il.AddInstruction(il.SetRegister(4, base, il.Add(4, il.Register(4, base), il.Register(4, index))));
        return true;
    }

    case XTENSA_INS_MADD_S:
    case XTENSA_INS_MSUB_S:
    case XTENSA_INS_MADDN_S:
    case XTENSA_INS_DIVN_S:
    {
        il.AddInstruction(il.Unimplemented());
        return true;
    }

    case XTENSA_INS_ROUND_S:
    case XTENSA_INS_TRUNC_S:
    case XTENSA_INS_FLOOR_S:
    case XTENSA_INS_CEIL_S:
    case XTENSA_INS_FLOAT_S:
    case XTENSA_INS_UFLOAT_S:
    case XTENSA_INS_UTRUNC_S:
    {
        il.AddInstruction(il.Unimplemented());
        return true;
    }

    case XTENSA_INS_RFR:
    case XTENSA_INS_WFR:
    case XTENSA_INS_CONST_S:
    {
        il.AddInstruction(il.Unimplemented());
        return true;
    }

    case XTENSA_INS_UN_S:
    {
        if (insn.operand_count != 3 || insn.operands[0].type != XTENSA_OP_BREG
            || insn.operands[1].type != XTENSA_OP_FPREG || insn.operands[2].type != XTENSA_OP_FPREG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest_flag = FLAG_B0 + insn.operands[0].reg;
        uint32_t src1 = fp_reg(insn.operands[1]);
        uint32_t src2 = fp_reg(insn.operands[2]);

        il.AddInstruction(
            il.SetFlag(dest_flag, il.FloatCompareUnordered(4, il.Register(4, src1), il.Register(4, src2))));
        return true;
    }

    case XTENSA_INS_OEQ_S:
    {
        if (insn.operand_count != 3 || insn.operands[0].type != XTENSA_OP_BREG
            || insn.operands[1].type != XTENSA_OP_FPREG || insn.operands[2].type != XTENSA_OP_FPREG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest_flag = FLAG_B0 + insn.operands[0].reg;
        uint32_t src1 = fp_reg(insn.operands[1]);
        uint32_t src2 = fp_reg(insn.operands[2]);

        il.AddInstruction(il.SetFlag(dest_flag, il.FloatCompareEqual(4, il.Register(4, src1), il.Register(4, src2))));
        return true;
    }

    case XTENSA_INS_UEQ_S:
    {
        if (insn.operand_count != 3 || insn.operands[0].type != XTENSA_OP_BREG
            || insn.operands[1].type != XTENSA_OP_FPREG || insn.operands[2].type != XTENSA_OP_FPREG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest_flag = FLAG_B0 + insn.operands[0].reg;
        uint32_t src1 = fp_reg(insn.operands[1]);
        uint32_t src2 = fp_reg(insn.operands[2]);

        // UEQ = Unordered OR Equal
        il.AddInstruction(il.SetFlag(dest_flag,
            il.Or(0, il.FloatCompareUnordered(4, il.Register(4, src1), il.Register(4, src2)),
                il.FloatCompareEqual(4, il.Register(4, src1), il.Register(4, src2)))));
        return true;
    }

    case XTENSA_INS_OLT_S:
    {
        if (insn.operand_count != 3 || insn.operands[0].type != XTENSA_OP_BREG
            || insn.operands[1].type != XTENSA_OP_FPREG || insn.operands[2].type != XTENSA_OP_FPREG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest_flag = FLAG_B0 + insn.operands[0].reg;
        uint32_t src1 = fp_reg(insn.operands[1]);
        uint32_t src2 = fp_reg(insn.operands[2]);

        il.AddInstruction(
            il.SetFlag(dest_flag, il.FloatCompareLessThan(4, il.Register(4, src1), il.Register(4, src2))));
        return true;
    }

    case XTENSA_INS_ULT_S:
    {
        if (insn.operand_count != 3 || insn.operands[0].type != XTENSA_OP_BREG
            || insn.operands[1].type != XTENSA_OP_FPREG || insn.operands[2].type != XTENSA_OP_FPREG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest_flag = FLAG_B0 + insn.operands[0].reg;
        uint32_t src1 = fp_reg(insn.operands[1]);
        uint32_t src2 = fp_reg(insn.operands[2]);

        // ULT = Unordered OR Less Than
        il.AddInstruction(il.SetFlag(dest_flag,
            il.Or(0, il.FloatCompareUnordered(4, il.Register(4, src1), il.Register(4, src2)),
                il.FloatCompareLessThan(4, il.Register(4, src1), il.Register(4, src2)))));
        return true;
    }

    case XTENSA_INS_OLE_S:
    {
        if (insn.operand_count != 3 || insn.operands[0].type != XTENSA_OP_BREG
            || insn.operands[1].type != XTENSA_OP_FPREG || insn.operands[2].type != XTENSA_OP_FPREG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest_flag = FLAG_B0 + insn.operands[0].reg;
        uint32_t src1 = fp_reg(insn.operands[1]);
        uint32_t src2 = fp_reg(insn.operands[2]);

        il.AddInstruction(
            il.SetFlag(dest_flag, il.FloatCompareLessEqual(4, il.Register(4, src1), il.Register(4, src2))));
        return true;
    }

    case XTENSA_INS_ULE_S:
    {
        if (insn.operand_count != 3 || insn.operands[0].type != XTENSA_OP_BREG
            || insn.operands[1].type != XTENSA_OP_FPREG || insn.operands[2].type != XTENSA_OP_FPREG)
        {
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest_flag = FLAG_B0 + insn.operands[0].reg;
        uint32_t src1 = fp_reg(insn.operands[1]);
        uint32_t src2 = fp_reg(insn.operands[2]);

        // ULE = Unordered OR Less Than or Equal
        il.AddInstruction(il.SetFlag(dest_flag,
            il.Or(0, il.FloatCompareUnordered(4, il.Register(4, src1), il.Register(4, src2)),
                il.FloatCompareLessEqual(4, il.Register(4, src1), il.Register(4, src2)))));
        return true;
    }

    case XTENSA_INS_MOVEQZ_S:
    case XTENSA_INS_MOVNEZ_S:
    case XTENSA_INS_MOVLTZ_S:
    case XTENSA_INS_MOVGEZ_S:
    case XTENSA_INS_MOVF_S:
    case XTENSA_INS_MOVT_S:
    {
        il.AddInstruction(il.Unimplemented());
        return true;
    }

    case XTENSA_INS_DIV0_S:
    case XTENSA_INS_RECIP0_S:
    case XTENSA_INS_SQRT0_S:
    case XTENSA_INS_RSQRT0_S:
    case XTENSA_INS_NEXP01_S:
    case XTENSA_INS_MKSADJ_S:
    case XTENSA_INS_MKDADJ_S:
    case XTENSA_INS_ADDEXP_S:
    case XTENSA_INS_ADDEXPM_S:
    {
        il.AddInstruction(il.Unimplemented());
        return true;
    }

    default:
        break;
    }

    return false;
}
