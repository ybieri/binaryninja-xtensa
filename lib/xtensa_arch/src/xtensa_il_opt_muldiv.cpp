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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src1 = insn.operands[1].reg;
            uint32_t src2 = insn.operands[2].reg;

            // AR[r] = AR[s] % AR[t] (unsigned remainder)
            // Note: Division by zero should trigger an exception, but LLIL doesn't have
            // explicit exception handling, so we proceed with the operation
            il.AddInstruction(il.SetRegister(4, dest,
                il.ModUnsigned(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            ));

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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src1 = insn.operands[1].reg;
            uint32_t src2 = insn.operands[2].reg;

            // AR[r] = AR[s] / AR[t] (unsigned division quotient)
            // Note: Division by zero should trigger an exception, but LLIL doesn't have
            // explicit exception handling, so we proceed with the operation
            il.AddInstruction(il.SetRegister(4, dest,
                il.DivUnsigned(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            ));

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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src1 = insn.operands[1].reg;
            uint32_t src2 = insn.operands[2].reg;

            // AR[r] = AR[s] / AR[t] (signed division quotient)
            il.AddInstruction(il.SetRegister(4, dest,
                il.DivSigned(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            ));

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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src1 = insn.operands[1].reg;
            uint32_t src2 = insn.operands[2].reg;

            // AR[r] = AR[s] % AR[t] (signed remainder)
            il.AddInstruction(il.SetRegister(4, dest,
                il.ModSigned(4,
                    il.Register(4, src1),
                    il.Register(4, src2)
                )
            ));

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

            uint32_t dest = insn.operands[0].reg;
            uint32_t src1 = insn.operands[1].reg;
            uint32_t src2 = insn.operands[2].reg;

            // AR[r] = lower 32 bits of (AR[s] * AR[t])
            // MulsDoublePrecUnsigned returns 64-bit result; LowPart extracts low 32 bits
            il.AddInstruction(il.SetRegister(4, dest,
                il.LowPart(4,
                    il.MultDoublePrecUnsigned(4,
                        il.Register(4, src1),
                        il.Register(4, src2)
                    )
                )
            ));

            return true;
        }

        default: break;
    }

    return false;
}