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
        // EXTUI - Extract Unsigned Immediate
        // Operation: AR[r] = (AR[t] >> shiftimm) & ((1 << maskimm) - 1)
        // Format: EXTUI ar, at, shiftimm, maskimm
        case XTENSA_INS_EXTUI:
        {
            if (insn.operand_count != 4)
            {
                LogWarn("EXTUI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG ||
                insn.operands[1].type != XTENSA_OP_REG ||
                insn.operands[2].type != XTENSA_OP_IMM ||
                insn.operands[3].type != XTENSA_OP_IMM)
            {
                LogWarn("EXTUI instruction at 0x%" PRIx64 " has unexpected operand types", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t dest = insn.operands[0].reg;
            uint32_t src = insn.operands[1].reg;
            int32_t shiftimm = insn.operands[2].imm;
            int32_t maskimm = insn.operands[3].imm;

            // Validate maskimm range (1-16)
            if (maskimm < 1 || maskimm > 16)
            {
                LogWarn("EXTUI instruction at 0x%" PRIx64 " has invalid maskimm: %d", addr, maskimm);
                il.AddInstruction(il.Undefined());
                return true;
            }

            // Calculate mask: (1 << maskimm) - 1
            // For maskimm=16, we need special handling since (1<<16) - 1 = 0xFFFF for 32-bit
            uint32_t mask;
            if (maskimm == 16) {
                mask = 0xFFFF;
            } else {
                mask = (1u << maskimm) - 1;
            }

            // AR[r] = (AR[t] >> shiftimm) & mask
            il.AddInstruction(il.SetRegister(4, dest,
                il.And(4,
                    il.LogicalShiftRight(4,
                        il.Register(4, src),
                        il.Const(4, shiftimm)
                    ),
                    il.Const(4, mask)
                )
            ));

            return true;
        }

        // SLLI - Shift Left Logical Immediate
        // Operation: AR[r] = AR[s] << sa
        // Format: SLLI ar, as, 1..31
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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src = insn.operands[1].reg;
            int32_t shift_amount = insn.operands[2].imm;

            // Validate shift amount range (should be 1-31, 0 is undefined/reserved)
            if (shift_amount < 1 || shift_amount > 31)
            {
                LogWarn("SLLI instruction at 0x%" PRIx64 " has invalid shift amount: %d", addr, shift_amount);
                il.AddInstruction(il.Undefined());
                return true;
            }

            // AR[r] = AR[s] << sa
            il.AddInstruction(il.SetRegister(4, dest,
                il.ShiftLeft(4,
                    il.Register(4, src),
                    il.Const(4, shift_amount)
                )
            ));

            return true;
        }

        // SRLI - Shift Right Logical Immediate
        // Operation: AR[r] = AR[t] >> sa (logical shift, zero-fill)
        // Format: SRLI ar, at, 0..15
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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src = insn.operands[1].reg;
            int32_t shift_amount = insn.operands[2].imm;

            // Validate shift amount range (0-15 per ISA spec)
            if (shift_amount < 0 || shift_amount > 15)
            {
                LogWarn("SRLI instruction at 0x%" PRIx64 " has invalid shift amount: %d", addr, shift_amount);
                il.AddInstruction(il.Undefined());
                return true;
            }

            // AR[r] = AR[t] >> sa (logical shift right, zero-fill)
            il.AddInstruction(il.SetRegister(4, dest,
                il.LogicalShiftRight(4,
                    il.Register(4, src),
                    il.Const(4, shift_amount)
                )
            ));

            return true;
        }

        // SRAI - Shift Right Arithmetic Immediate
        // Operation: AR[r] = AR[t] >> sa (arithmetic shift, sign-extend)
        // Format: SRAI ar, at, 0..31
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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src = insn.operands[1].reg;
            int32_t shift_amount = insn.operands[2].imm;

            // Validate shift amount range (0-31)
            if (shift_amount < 0 || shift_amount > 31)
            {
                LogWarn("SRAI instruction at 0x%" PRIx64 " has invalid shift amount: %d", addr, shift_amount);
                il.AddInstruction(il.Undefined());
                return true;
            }

            // AR[r] = AR[t] >> sa (arithmetic shift right, sign-extend)
            il.AddInstruction(il.SetRegister(4, dest,
                il.ArithShiftRight(4,
                    il.Register(4, src),
                    il.Const(4, shift_amount)
                )
            ));

            return true;
        }

        // SRC - Shift Right Combined (Funnel Shift)
        // Operation: AR[r] = (AR[s] concat AR[t]) >> SAR
        // This is a 64-bit funnel shift taking high 32 bits from AR[s] and low 32 bits from AR[t]
        // Format: SRC ar, as, at
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

            uint32_t dest = insn.operands[0].reg;  // ar
            uint32_t src1 = insn.operands[1].reg;  // as (high bits)
            uint32_t src2 = insn.operands[2].reg;  // at (low bits)

            // SRC: AR[r] = ((AR[s] << 32) | AR[t]) >> SAR
            // Equivalent to: (AR[s] << (32 - SAR)) | (AR[t] >> SAR)
            uint32_t temp_sar = LLIL_TEMP(0);
            uint32_t temp_high = LLIL_TEMP(1);
            uint32_t temp_low = LLIL_TEMP(2);

            // temp_sar = SAR & 0x1F (only 5 bits used)
            il.AddInstruction(il.SetRegister(4, temp_sar,
                il.And(4, il.Register(4, REG_SAR), il.Const(4, 0x1F))));

            // temp_high = AR[s] << (32 - temp_sar)
            il.AddInstruction(il.SetRegister(4, temp_high,
                il.ShiftLeft(4,
                    il.Register(4, src1),
                    il.Sub(4, il.Const(4, 32), il.Register(4, temp_sar)))));

            // temp_low = AR[t] >> temp_sar
            il.AddInstruction(il.SetRegister(4, temp_low,
                il.LogicalShiftRight(4,
                    il.Register(4, src2),
                    il.Register(4, temp_sar))));

            // dest = temp_high | temp_low
            il.AddInstruction(il.SetRegister(4, dest,
                il.Or(4, il.Register(4, temp_high), il.Register(4, temp_low))));

            return true;
        }

        // SRA - Shift Right Arithmetic (by SAR)
        // Operation: AR[r] = (signed)AR[t] >> SAR
        // Format: SRA ar, at
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

            uint32_t dest = insn.operands[0].reg;  // ar
            uint32_t src = insn.operands[1].reg;   // at

            // SRA uses SAR[5:0] for shift amount (only low 5 bits matter for 32-bit shift)
            il.AddInstruction(il.SetRegister(4, dest,
                il.ArithShiftRight(4,
                    il.Register(4, src),
                    il.And(4, il.Register(4, REG_SAR), il.Const(4, 0x1F)))));

            return true;
        }

        // SLL - Shift Left Logical
        // Operation: AR[r] = AR[s] << SAR[5:0]
        // Format: SLL ar, as
        // Note: SAR register must be set by SSL instruction before SLL
        case XTENSA_INS_SLL:
        {
            if (insn.operand_count != 2)
            {
                LogWarn("SLL instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_REG)
            {
                LogWarn("SLL instruction at 0x%" PRIx64 " has non-register operands", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t dest = insn.operands[0].reg;  // ar (destination)
            uint32_t src = insn.operands[1].reg;   // as (source)

            // SLL uses SAR[5:0] for shift amount
            // If shift amount >= 32, result is 0
            
            uint32_t temp_shift = LLIL_TEMP(1);
            
            // temp_shift = SAR & 0x3F
            il.AddInstruction(il.SetRegister(4, temp_shift,
                il.And(4,
                    il.Register(4, REG_SAR),
                    il.Const(4, 0x3F)
                )
            ));

            LowLevelILLabel shiftLabel, zeroLabel, doneLabel;

            // if (temp_shift < 32)
            il.AddInstruction(il.If(
                il.CompareUnsignedLessThan(4,
                    il.Register(4, temp_shift),
                    il.Const(4, 32)
                ),
                shiftLabel,
                zeroLabel
            ));

            // Shift branch: dest = src << temp_shift
            il.MarkLabel(shiftLabel);
            il.AddInstruction(il.SetRegister(4, dest,
                il.ShiftLeft(4,
                    il.Register(4, src),
                    il.Register(4, temp_shift)
                )
            ));
            il.AddInstruction(il.Goto(doneLabel));

            // Zero branch: dest = 0
            il.MarkLabel(zeroLabel);
            il.AddInstruction(il.SetRegister(4, dest, il.Const(4, 0)));

            il.MarkLabel(doneLabel);

            return true;
        }

        // SRL - Shift Right Logical (by SAR)
        // Operation: AR[r] = AR[t] >> SAR (zero-fill)
        // Format: SRL ar, at
        case XTENSA_INS_SRL:
        {
            if (insn.operand_count != 2)
            {
                LogWarn("SRL instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_REG)
            {
                LogWarn("SRL instruction at 0x%" PRIx64 " has non-register operands", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t dest = insn.operands[0].reg;  // ar
            uint32_t src = insn.operands[1].reg;   // at

            // SRL uses SAR[5:0] for shift amount (only low 5 bits matter for 32-bit shift)
            il.AddInstruction(il.SetRegister(4, dest,
                il.LogicalShiftRight(4,
                    il.Register(4, src),
                    il.And(4, il.Register(4, REG_SAR), il.Const(4, 0x1F)))));

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

            uint32_t src = insn.operands[0].reg;

            // SAR = 32 - ((AR[s] & 3) << 3)
            il.AddInstruction(il.SetRegister(4, REG_SAR,
                il.Sub(4,
                    il.Const(4, 32),
                    il.ShiftLeft(4,
                        il.And(4, il.Register(4, src), il.Const(4, 3)),
                        il.Const(4, 3)))));

            return true;
        }

        // SSA8L - Set Shift Amount for Little-endian Byte Align
        // Operation: SAR = AR[s][1:0] << 3
        // Used for little-endian byte alignment operations
        // Format: SSA8L as
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

            uint32_t src = insn.operands[0].reg;

            // SAR = (AR[s] & 3) << 3
            il.AddInstruction(il.SetRegister(4, REG_SAR,
                il.ShiftLeft(4,
                    il.And(4, il.Register(4, src), il.Const(4, 3)),
                    il.Const(4, 3))));

            return true;
        }

        // SSR - Set Shift Amount for Right Shift
        // Operation: SAR = AR[s][4:0]
        // Sets shift amount register for subsequent SRL/SRA instructions
        // Format: SSR as
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

            uint32_t src = insn.operands[0].reg;

            // SAR = AR[s] & 0x1F (only low 5 bits)
            il.AddInstruction(il.SetRegister(4, REG_SAR,
                il.And(4, il.Register(4, src), il.Const(4, 0x1F))));

            return true;
        }

        // SSL - Set Shift Amount for Left Shift
        // Operation: SAR = 32 - AR[s][4:0]
        // Sets shift amount register for subsequent SLL instruction
        // Format: SSL as
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

            uint32_t src = insn.operands[0].reg;

            // SAR = 32 - (AR[s] & 0x1F)
            il.AddInstruction(il.SetRegister(4, REG_SAR,
                il.Sub(4,
                    il.Const(4, 32),
                    il.And(4, il.Register(4, src), il.Const(4, 0x1F)))));

            return true;
        }

        // SSAI - Set Shift Amount Immediate
        // Operation: SAR = imm5
        // Sets shift amount register to immediate value
        // Format: SSAI 0..31
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

            int32_t imm = insn.operands[0].imm;

            // SAR = imm (0-31)
            il.AddInstruction(il.SetRegister(4, REG_SAR, il.Const(4, imm & 0x1F)));

            return true;
        }

        default: break;
    }

    return false;
}
