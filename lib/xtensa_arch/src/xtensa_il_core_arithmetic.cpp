#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 3.8.7
    Title: Arithmetic Instructions
*/

LLIL_LIFTER(core_arithmetic)
{
    switch (insn.id)
    {
        // ADD - Add two registers
        // Operation: AR[r] = AR[s] + AR[t]
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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src1 = insn.operands[1].reg;
            uint32_t src2 = insn.operands[2].reg;

            il.AddInstruction(il.SetRegister(4, dest,
                il.Add(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            ));

            return true;
        }

        // ADDX2 - Add with shift by 1
        // Operation: AR[r] = (AR[s] << 1) + AR[t]
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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src1 = insn.operands[1].reg;
            uint32_t src2 = insn.operands[2].reg;

            // AR[r] = (AR[s] << 1) + AR[t]
            il.AddInstruction(il.SetRegister(4, dest,
                il.Add(4,
                    il.ShiftLeft(4,
                        il.Register(4, src1),
                        il.Const(4, 1)
                    ),
                    il.Register(4, src2)
                )
            ));

            return true;
        }

        // ADDX4 - Add with shift by 2
        // Operation: AR[r] = (AR[s] << 2) + AR[t]
        // Format: ADDX4 ar, as, at
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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src1 = insn.operands[1].reg;
            uint32_t src2 = insn.operands[2].reg;

            // AR[r] = (AR[s] << 2) + AR[t]
            il.AddInstruction(il.SetRegister(4, dest,
                il.Add(4,
                    il.ShiftLeft(4,
                        il.Register(4, src1),
                        il.Const(4, 2)
                    ),
                    il.Register(4, src2)
                )
            ));

            return true;
        }

        // ADDX8 - Add with shift by 3
        // Operation: AR[r] = (AR[s] << 3) + AR[t]
        // Format: ADDX8 ar, as, at
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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src1 = insn.operands[1].reg;
            uint32_t src2 = insn.operands[2].reg;

            // AR[r] = (AR[s] << 3) + AR[t]
            il.AddInstruction(il.SetRegister(4, dest,
                il.Add(4,
                    il.ShiftLeft(4,
                        il.Register(4, src1),
                        il.Const(4, 3)
                    ),
                    il.Register(4, src2)
                )
            ));

            return true;
        }

        // SUB - Subtract two registers
        // Operation: AR[r] = AR[s] - AR[t]
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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src1 = insn.operands[1].reg;
            uint32_t src2 = insn.operands[2].reg;

            il.AddInstruction(il.SetRegister(4, dest,
                il.Sub(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            ));

            return true;
        }

        // SUBX2 - Subtract with shift by 1
        // Operation: AR[r] = (AR[s] << 1) - AR[t]
        // Format: SUBX2 ar, as, at
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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src1 = insn.operands[1].reg;
            uint32_t src2 = insn.operands[2].reg;

            // AR[r] = (AR[s] << 1) - AR[t]
            il.AddInstruction(il.SetRegister(4, dest,
                il.Sub(4,
                    il.ShiftLeft(4,
                        il.Register(4, src1),
                        il.Const(4, 1)
                    ),
                    il.Register(4, src2)
                )
            ));

            return true;
        }

        // SUBX4 - Subtract with shift by 2
        // Operation: AR[r] = (AR[s] << 2) - AR[t]
        // Format: SUBX4 ar, as, at
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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src1 = insn.operands[1].reg;
            uint32_t src2 = insn.operands[2].reg;

            // AR[r] = (AR[s] << 2) - AR[t]
            il.AddInstruction(il.SetRegister(4, dest,
                il.Sub(4,
                    il.ShiftLeft(4,
                        il.Register(4, src1),
                        il.Const(4, 2)
                    ),
                    il.Register(4, src2)
                )
            ));

            return true;
        }

        // SUBX8 - Subtract with shift by 3
        // Operation: AR[r] = (AR[s] << 3) - AR[t]
        // Format: SUBX8 ar, as, at
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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src1 = insn.operands[1].reg;
            uint32_t src2 = insn.operands[2].reg;

            // AR[r] = (AR[s] << 3) - AR[t]
            il.AddInstruction(il.SetRegister(4, dest,
                il.Sub(4,
                    il.ShiftLeft(4,
                        il.Register(4, src1),
                        il.Const(4, 3)
                    ),
                    il.Register(4, src2)
                )
            ));

            return true;
        }

        // NEG - Negate
        // Operation: AR[r] = 0 - AR[t]
        // Format: NEG ar, at
        case XTENSA_INS_NEG:
        {
            if (insn.operand_count != 2)
            {
                LogWarn("NEG instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG ||
                insn.operands[1].type != XTENSA_OP_REG)
            {
                LogWarn("NEG instruction at 0x%" PRIx64 " has non-register operands", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t dest = insn.operands[0].reg;
            uint32_t src = insn.operands[1].reg;

            // AR[r] = 0 - AR[t]
            il.AddInstruction(il.SetRegister(4, dest,
                il.Neg(4,
                    il.Register(4, src)
                )
            ));

            return true;
        }

        // ABS - Absolute Value
        // Operation: AR[r] = if AR[t]31 then -AR[t] else AR[t]
        // Format: ABS ar, at
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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src = insn.operands[1].reg;

            // AR[r] = if AR[t]31 then -AR[t] else AR[t]
            // Implement using control flow: if (src < 0) dest = -src; else dest = src;
            LowLevelILLabel trueLabel, falseLabel, doneLabel;

            // Compare src with 0
            il.AddInstruction(il.If(
                il.CompareSignedLessThan(4,
                    il.Register(4, src),
                    il.Const(4, 0)
                ),
                trueLabel,
                falseLabel
            ));

            // True branch: dest = -src
            il.MarkLabel(trueLabel);
            il.AddInstruction(il.SetRegister(4, dest,
                il.Neg(4, il.Register(4, src))
            ));
            il.AddInstruction(il.Goto(doneLabel));

            // False branch: dest = src
            il.MarkLabel(falseLabel);
            il.AddInstruction(il.SetRegister(4, dest,
                il.Register(4, src)
            ));

            // Done label
            il.MarkLabel(doneLabel);

            return true;
        }

        // ADDI - Add immediate to register
        // Operation: AR[t] = AR[s] + sign_extend(imm8)
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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src = insn.operands[1].reg;
            int32_t imm = insn.operands[2].imm;

            il.AddInstruction(il.SetRegister(4, dest,
                il.Add(4,
                    il.Register(4, src),
                    il.Const(4, imm)
                )
            ));

            return true;
        }

        // ADDMI - Add Immediate with Shift by 8
        // Operation: AR[t] = AR[s] + (sign_extend(imm8) << 8)
        // Format: ADDMI at, as, -32768..32512
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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src = insn.operands[1].reg;
            int32_t imm = insn.operands[2].imm;

            // ADDMI: The immediate is already shifted by 8 in the decoder
            // Decoder does: sign_extend_operand(imm8, 8) << 8
            il.AddInstruction(il.SetRegister(4, dest,
                il.Add(4,
                    il.Register(4, src),
                    il.Const(4, imm)
                )
            ));

            return true;
        }

        // TODO: SALT and SALTU is unimplemented in decoder.

        default: break;
    }

    return false;
}
