#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 4.3.8
    Title: Miscellaneous Operations Option
*/

LLIL_LIFTER(opt_misc)
{
    switch (insn.id)
    {
        case XTENSA_INS_MIN:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("MIN instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG ||
                insn.operands[1].type != XTENSA_OP_REG ||
                insn.operands[2].type != XTENSA_OP_REG)
            {
                LogWarn("MIN instruction at 0x%" PRIx64 " has non-register operands", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t dest = insn.operands[0].reg;
            uint32_t s = insn.operands[1].reg;
            uint32_t t = insn.operands[2].reg;

            LowLevelILLabel trueLabel, falseLabel, doneLabel;

            // if (AR[s] < AR[t]) goto trueLabel else goto falseLabel
            il.AddInstruction(il.If(
                il.CompareSignedLessThan(4, il.Register(4, s), il.Register(4, t)),
                trueLabel,
                falseLabel
            ));

            // True branch: dest = s
            il.MarkLabel(trueLabel);
            il.AddInstruction(il.SetRegister(4, dest, il.Register(4, s)));
            il.AddInstruction(il.Goto(doneLabel));

            // False branch: dest = t
            il.MarkLabel(falseLabel);
            il.AddInstruction(il.SetRegister(4, dest, il.Register(4, t)));

            il.MarkLabel(doneLabel);
            return true;
        }

        case XTENSA_INS_MINU:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("MINU instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG ||
                insn.operands[1].type != XTENSA_OP_REG ||
                insn.operands[2].type != XTENSA_OP_REG)
            {
                LogWarn("MINU instruction at 0x%" PRIx64 " has non-register operands", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t dest = insn.operands[0].reg;
            uint32_t s = insn.operands[1].reg;
            uint32_t t = insn.operands[2].reg;

            LowLevelILLabel trueLabel, falseLabel, doneLabel;

            // if (AR[s] <u AR[t]) goto trueLabel else goto falseLabel
            il.AddInstruction(il.If(
                il.CompareUnsignedLessThan(4, il.Register(4, s), il.Register(4, t)),
                trueLabel,
                falseLabel
            ));

            // True branch: dest = s
            il.MarkLabel(trueLabel);
            il.AddInstruction(il.SetRegister(4, dest, il.Register(4, s)));
            il.AddInstruction(il.Goto(doneLabel));

            // False branch: dest = t
            il.MarkLabel(falseLabel);
            il.AddInstruction(il.SetRegister(4, dest, il.Register(4, t)));

            il.MarkLabel(doneLabel);
            return true;
        }

        case XTENSA_INS_MAX:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("MAX instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG ||
                insn.operands[1].type != XTENSA_OP_REG ||
                insn.operands[2].type != XTENSA_OP_REG)
            {
                LogWarn("MAX instruction at 0x%" PRIx64 " has non-register operands", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t dest = insn.operands[0].reg;
            uint32_t s = insn.operands[1].reg;
            uint32_t t = insn.operands[2].reg;

            LowLevelILLabel trueLabel, falseLabel, doneLabel;

            // if (AR[s] > AR[t]) goto trueLabel else goto falseLabel
            il.AddInstruction(il.If(
                il.CompareSignedGreaterThan(4, il.Register(4, s), il.Register(4, t)),
                trueLabel,
                falseLabel
            ));

            // True branch: dest = s
            il.MarkLabel(trueLabel);
            il.AddInstruction(il.SetRegister(4, dest, il.Register(4, s)));
            il.AddInstruction(il.Goto(doneLabel));

            // False branch: dest = t
            il.MarkLabel(falseLabel);
            il.AddInstruction(il.SetRegister(4, dest, il.Register(4, t)));

            il.MarkLabel(doneLabel);
            return true;
        }

        case XTENSA_INS_MAXU:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("MAXU instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG ||
                insn.operands[1].type != XTENSA_OP_REG ||
                insn.operands[2].type != XTENSA_OP_REG)
            {
                LogWarn("MAXU instruction at 0x%" PRIx64 " has non-register operands", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t dest = insn.operands[0].reg;
            uint32_t s = insn.operands[1].reg;
            uint32_t t = insn.operands[2].reg;

            LowLevelILLabel trueLabel, falseLabel, doneLabel;

            // if (AR[s] >u AR[t]) goto trueLabel else goto falseLabel
            il.AddInstruction(il.If(
                il.CompareUnsignedGreaterThan(4, il.Register(4, s), il.Register(4, t)),
                trueLabel,
                falseLabel
            ));

            // True branch: dest = s
            il.MarkLabel(trueLabel);
            il.AddInstruction(il.SetRegister(4, dest, il.Register(4, s)));
            il.AddInstruction(il.Goto(doneLabel));

            // False branch: dest = t
            il.MarkLabel(falseLabel);
            il.AddInstruction(il.SetRegister(4, dest, il.Register(4, t)));

            il.MarkLabel(doneLabel);
            return true;
        }

        case XTENSA_INS_CLAMPS:
        case XTENSA_INS_NSA:
        case XTENSA_INS_NSAU:
        {
            il.AddInstruction(il.Unimplemented());
            return true;
        }

        case XTENSA_INS_SEXT:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("SEXT instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG ||
                insn.operands[1].type != XTENSA_OP_REG ||
                insn.operands[2].type != XTENSA_OP_IMM)
            {
                LogWarn("SEXT instruction at 0x%" PRIx64 " has unexpected operand types", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t dest = insn.operands[0].reg;
            uint32_t src = insn.operands[1].reg;
            int32_t imm = insn.operands[2].imm;  // field_size - 1

            // Calculate actual field size
            uint32_t field_bits = imm + 1;

            // Validate field size (must be 1-32)
            if (field_bits < 1 || field_bits > 32)
            {
                LogWarn("SEXT instruction at 0x%" PRIx64 " has invalid field size: %d", addr, field_bits);
                il.AddInstruction(il.Unimplemented());
                return true;
            }

            if (field_bits == 32) {
                // Special case: sign-extend 32 bits is just a copy
                il.AddInstruction(il.SetRegister(4, dest,
                    il.Register(4, src)
                ));
            } else {
                // Sign-extend by shifting left to move MSB to position 31, then arithmetic shift right
                // Example: SEXT ar, as, 7 (field_bits=8)
                // 1. Shift left by (32 - 8) = 24 bits to align MSB with bit 31
                // 2. Arithmetic shift right by 24 bits to sign-extend
                uint32_t shift_amount = 32 - field_bits;

                il.AddInstruction(il.SetRegister(4, dest,
                    il.ArithShiftRight(4,
                        il.ShiftLeft(4,
                            il.Register(4, src),
                            il.Const(4, shift_amount)
                        ),
                        il.Const(4, shift_amount)
                    )
                ));
            }

            return true;
        }

        default: break;
    }

    return false;
}