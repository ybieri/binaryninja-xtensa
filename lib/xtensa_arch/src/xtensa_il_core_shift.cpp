#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 3.8.9
    Title: Shift Instructions
*/

LLIL_LIFTER(core_shift)
{
    switch (insn.id)
    {
    case XTENSA_INS_EXTUI:
    {
        if (insn.operand_count != 4)
        {
            LogWarn("EXTUI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_IMM || insn.operands[3].type != XTENSA_OP_IMM)
        {
            LogWarn("EXTUI instruction at 0x%" PRIx64 " has unexpected operand types", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src = REG_A0 + insn.operands[1].reg;
        int32_t shiftimm = insn.operands[2].imm;
        int32_t maskimm = insn.operands[3].imm;

        if (maskimm < 1 || maskimm > 16)
        {
            LogWarn("EXTUI instruction at 0x%" PRIx64 " has invalid maskimm: %d", addr, maskimm);
            il.AddInstruction(il.Undefined());
            return true;
        }

        ExprId lop;

        if (shiftimm == 0)
            lop = il.Register(4, src);
        else
            lop = il.LogicalShiftRight(4, il.Register(4, src), il.Const(4, shiftimm));

        uint32_t mask = (1u << maskimm) - 1;

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.And(4, lop, il.Const(4, mask))
            )
        );

        return true;
    }

    case XTENSA_INS_SLLI:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("SLLI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_IMM)
        {
            LogWarn("SLLI instruction at 0x%" PRIx64 " has unexpected operand types", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src = REG_A0 + insn.operands[1].reg;
        int32_t shift_amount = insn.operands[2].imm;

        // Validate shift amount range (should be 1-31, 0 is undefined/reserved)
        if (shift_amount < 1 || shift_amount > 31)
        {
            LogDebug("SLLI instruction at 0x%" PRIx64 " has invalid shift amount: %d", addr, shift_amount);
            il.AddInstruction(il.Undefined());
            return true;
        }

        // AR[r] = AR[s] << sa
        il.AddInstruction(il.SetRegister(4, dest, il.ShiftLeft(4, il.Register(4, src), il.Const(4, shift_amount))));

        return true;
    }

    case XTENSA_INS_SRLI:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("SRLI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_IMM)
        {
            LogWarn("SRLI instruction at 0x%" PRIx64 " has unexpected operand types", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src = REG_A0 + insn.operands[1].reg;
        int32_t shift_amount = insn.operands[2].imm;

        if (shift_amount < 0 || shift_amount > 15)
        {
            LogWarn("SRLI instruction at 0x%" PRIx64 " has invalid shift amount: %d", addr, shift_amount);
            il.AddInstruction(il.Undefined());
            return true;
        }

        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.LogicalShiftRight(4,
                    il.Register(4, src),
                    il.Const(4, shift_amount)
                )
            )
        );

        return true;
    }

    case XTENSA_INS_SRAI:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("SRAI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_IMM)
        {
            LogWarn("SRAI instruction at 0x%" PRIx64 " has unexpected operand types", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src = REG_A0 + insn.operands[1].reg;
        int32_t shift_amount = insn.operands[2].imm;

        // Validate shift amount range (0-31)
        if (shift_amount < 0 || shift_amount > 31)
        {
            LogWarn("SRAI instruction at 0x%" PRIx64 " has invalid shift amount: %d", addr, shift_amount);
            il.AddInstruction(il.Undefined());
            return true;
        }

        // AR[r] = AR[t] >> sa (arithmetic shift right, sign-extend)
        il.AddInstruction(
            il.SetRegister(4, dest, il.ArithShiftRight(4, il.Register(4, src), il.Const(4, shift_amount))));

        return true;
    }

    case XTENSA_INS_SRC:
    {
        if (insn.operand_count != 3)
        {
            LogWarn("SRC instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG ||
            insn.operands[2].type != XTENSA_OP_REG)
        {
            LogWarn("SRC instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;  // ar
        uint32_t src1 = REG_A0 + insn.operands[1].reg;  // as (high bits)
        uint32_t src2 = REG_A0 + insn.operands[2].reg;  // at (low bits)

        // SRC: AR[r] = ((AR[s] << 32) | AR[t]) >> SAR
        // Equivalent to: (AR[s] << (32 - SAR)) | (AR[t] >> SAR)
        uint32_t temp_sar = LLIL_TEMP(0);
        uint32_t temp_high = LLIL_TEMP(1);
        uint32_t temp_low = LLIL_TEMP(2);

        // temp_sar = SAR & 0x1F (only 5 bits used)
        il.AddInstruction(il.SetRegister(4, temp_sar, il.And(4, il.Register(4, REG_SAR), il.Const(4, 0x1F))));

        // temp_high = AR[s] << (32 - temp_sar)
        il.AddInstruction(il.SetRegister(
            4, temp_high, il.ShiftLeft(4, il.Register(4, src1), il.Sub(4, il.Const(4, 32), il.Register(4, temp_sar)))));

        // temp_low = AR[t] >> temp_sar
        il.AddInstruction(
            il.SetRegister(4, temp_low, il.LogicalShiftRight(4, il.Register(4, src2), il.Register(4, temp_sar))));

        // dest = temp_high | temp_low
        il.AddInstruction(il.SetRegister(4, dest, il.Or(4, il.Register(4, temp_high), il.Register(4, temp_low))));

        return true;
    }

    case XTENSA_INS_SRA:
    {
        if (insn.operand_count != 2)
        {
            LogWarn("SRA instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_REG)
        {
            LogWarn("SRA instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;  // ar
        uint32_t src = REG_A0 + insn.operands[1].reg;   // at

        // SRA uses SAR[5:0] for shift amount (only low 5 bits matter for 32-bit shift)
        il.AddInstruction(il.SetRegister(4, dest,
            il.ArithShiftRight(4, il.Register(4, src), il.And(4, il.Register(4, REG_SAR), il.Const(4, 0x1F)))));

        return true;
    }

    case XTENSA_INS_SLL:
    {
        if (insn.operand_count != 2)
        {
            LogWarn("SLL instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG)
        {
            LogWarn("SLL instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;
        uint32_t src = REG_A0 + insn.operands[1].reg;

        uint32_t temp_shift = LLIL_TEMP(1);

        // shift = sar & 0b11111
        il.AddInstruction(
            il.SetRegister(4,
                temp_shift,
                il.And(4,
                    il.Register(4, REG_SAR),
                    il.Const(4, 0b11111))
                )
        );

        // dest = src << (32 - shift)
        il.AddInstruction(
            il.SetRegister(4,
                dest,
                il.ShiftLeft(4,
                    il.Register(4, src),
                    il.Sub(4,
                        il.Const(4, 32),
                        il.Register(4, temp_shift)
                    )
                )
            )
        );

        return true;
    }

    case XTENSA_INS_SRL:
    {
        if (insn.operand_count != 2)
        {
            LogWarn("SRL instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG ||
            insn.operands[1].type != XTENSA_OP_REG)
        {
            LogWarn("SRL instruction at 0x%" PRIx64 " has non-register operands", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t dest = REG_A0 + insn.operands[0].reg;  // ar
        uint32_t src = REG_A0 + insn.operands[1].reg;   // at

        // SRL uses SAR[5:0] for shift amount (only low 5 bits matter for 32-bit shift)
        il.AddInstruction(il.SetRegister(4, dest,
            il.LogicalShiftRight(4, il.Register(4, src), il.And(4, il.Register(4, REG_SAR), il.Const(4, 0x1F)))));

        return true;
    }

    // SSA8B - Set Shift Amount for Big-endian Byte Align
    // Operation: SAR = 32 - (AR[s][1:0] << 3)
    // Used for big-endian byte alignment operations
    // Format: SSA8B as
    case XTENSA_INS_SSA8B:
    {
        if (insn.operand_count != 1)
        {
            LogWarn("SSA8B instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG)
        {
            LogWarn("SSA8B instruction at 0x%" PRIx64 " has non-register operand", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t src = REG_A0 + insn.operands[0].reg;

        // SAR = 32 - ((AR[s] & 3) << 3)
        il.AddInstruction(il.SetRegister(4, REG_SAR,
            il.Sub(
                4, il.Const(4, 32), il.ShiftLeft(4, il.And(4, il.Register(4, src), il.Const(4, 3)), il.Const(4, 3)))));

        return true;
    }

    case XTENSA_INS_SSA8L:
    {
        if (insn.operand_count != 1)
        {
            LogWarn("SSA8L instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG)
        {
            LogWarn("SSA8L instruction at 0x%" PRIx64 " has non-register operand", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t src = REG_A0 + insn.operands[0].reg;

        // SAR = (AR[s] & 3) << 3
        il.AddInstruction(il.SetRegister(
            4, REG_SAR, il.ShiftLeft(4, il.And(4, il.Register(4, src), il.Const(4, 3)), il.Const(4, 3))));

        return true;
    }

    case XTENSA_INS_SSR:
    {
        if (insn.operand_count != 1)
        {
            LogWarn("SSR instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG)
        {
            LogWarn("SSR instruction at 0x%" PRIx64 " has non-register operand", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t src = REG_A0 + insn.operands[0].reg;

        // SAR = AR[s] & 0x1F (only low 5 bits)
        il.AddInstruction(il.SetRegister(4, REG_SAR, il.And(4, il.Register(4, src), il.Const(4, 0x1F))));

        return true;
    }

    case XTENSA_INS_SSL:
    {
        if (insn.operand_count != 1)
        {
            LogWarn("SSL instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_REG)
        {
            LogWarn("SSL instruction at 0x%" PRIx64 " has non-register operand", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        uint32_t src = REG_A0 + insn.operands[0].reg;

        // SAR = 32 - (AR[s] & 0x1F)
        il.AddInstruction(
            il.SetRegister(4, REG_SAR, il.Sub(4, il.Const(4, 32), il.And(4, il.Register(4, src), il.Const(4, 0x1F)))));

        return true;
    }

    case XTENSA_INS_SSAI:
    {
        if (insn.operand_count != 1)
        {
            LogWarn("SSAI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
            il.AddInstruction(il.Undefined());
            return true;
        }
        if (insn.operands[0].type != XTENSA_OP_IMM)
        {
            LogWarn("SSAI instruction at 0x%" PRIx64 " has non-immediate operand", addr);
            il.AddInstruction(il.Undefined());
            return true;
        }

        int32_t imm = REG_A0 + insn.operands[0].imm;

        // SAR = imm (0-31)
        il.AddInstruction(il.SetRegister(4, REG_SAR, il.Const(4, imm & 0x1F)));

        return true;
    }

    default:
        break;
    }

    return false;
}
