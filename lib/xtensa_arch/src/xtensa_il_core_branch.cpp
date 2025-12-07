#include "xtensa_il.h"
#include "xtensa_arch.h"
#include <inttypes.h>

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 3.8.5
    Title: Conditional Branch Instructions
*/

LLIL_LIFTER(core_branch)
{
    switch (insn.id)
    {
        // BEQZ - Branch if Equal to Zero
        // Operation: if (AR[s] == 0) PC = PC + 4 + imm; else PC = PC + 3
        // Format: BEQZ as, imm
        case XTENSA_INS_BEQZ:
        {
            if (insn.operand_count != 2)
            {
                LogWarn("BEQZ instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BEQZ instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BEQZ instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint64_t target_addr = insn.operands[1].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareEqual(4, il.Register(4, s), il.Const(4, 0)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BNEZ - Branch if Not Equal to Zero
        // Operation: if (AR[s] != 0) PC = PC + 4 + imm; else PC = PC + 3
        // Format: BNEZ as, imm
        case XTENSA_INS_BNEZ:
        {
            if (insn.operand_count != 2)
            {
                LogWarn("BNEZ instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BNEZ instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BNEZ instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint64_t target_addr = insn.operands[1].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareNotEqual(4, il.Register(4, s), il.Const(4, 0)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BGEZ - Branch if Greater Than or Equal to Zero
        // Operation: if (AR[s] >= 0) PC = PC + 4 + imm; else PC = PC + 3
        // Format: BGEZ as, imm
        case XTENSA_INS_BGEZ:
        {
            if (insn.operand_count != 2)
            {
                LogWarn("BGEZ instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BGEZ instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BGEZ instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint64_t target_addr = insn.operands[1].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareSignedGreaterEqual(4, il.Register(4, s), il.Const(4, 0)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BLTZ - Branch if Less Than Zero
        // Operation: if (AR[s] < 0) PC = PC + 4 + imm; else PC = PC + 3
        // Format: BLTZ as, imm
        case XTENSA_INS_BLTZ:
        {
            if (insn.operand_count != 2)
            {
                LogWarn("BLTZ instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BLTZ instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BLTZ instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint64_t target_addr = insn.operands[1].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareSignedLessThan(4, il.Register(4, s), il.Const(4, 0)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BEQI - Branch if Equal Immediate
        // Operation: if (AR[s] == B4CONST[r]) PC = PC + 4 + sign_extend(imm8)
        // Format: BEQI as, b4const, offset
        case XTENSA_INS_BEQI:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BEQI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BEQI instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_IMM)
            {
                LogWarn("BEQI instruction at 0x%" PRIx64 " has non-immediate operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BEQI instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            int32_t imm = insn.operands[1].imm;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareEqual(4, il.Register(4, s), il.Const(4, imm)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BNEI - Branch if Not Equal Immediate
        // Operation: if (AR[s] != B4CONST[r]) PC = PC + 4 + sign_extend(imm8)
        // Format: BNEI as, b4const, offset
        case XTENSA_INS_BNEI:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BNEI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BNEI instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_IMM)
            {
                LogWarn("BNEI instruction at 0x%" PRIx64 " has non-immediate operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BNEI instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            int32_t imm = insn.operands[1].imm;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareNotEqual(4, il.Register(4, s), il.Const(4, imm)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BGEI - Branch if Greater Than or Equal Immediate
        // Operation: if (AR[s] >= imm) PC = PC + 4 + offset; else PC = PC + 3
        // Format: BGEI as, imm, offset
        case XTENSA_INS_BGEI:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BGEI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BGEI instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_IMM)
            {
                LogWarn("BGEI instruction at 0x%" PRIx64 " has non-immediate operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BGEI instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            int32_t imm = insn.operands[1].imm;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareSignedGreaterEqual(4, il.Register(4, s), il.Const(4, imm)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BLTI - Branch if Less Than Immediate
        // Operation: if (AR[s] < imm) PC = PC + 4 + offset; else PC = PC + 3
        // Format: BLTI as, imm, offset
        case XTENSA_INS_BLTI:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BLTI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BLTI instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_IMM)
            {
                LogWarn("BLTI instruction at 0x%" PRIx64 " has non-immediate operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BLTI instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            int32_t imm = insn.operands[1].imm;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareSignedLessThan(4, il.Register(4, s), il.Const(4, imm)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BGEUI - Branch if Greater Than or Equal Unsigned Immediate
        // Operation: if (AR[s] >= b4constu[r]) PC = PC + 4 + sign_extend(imm8)
        // Format: BGEUI as, b4constu, offset
        case XTENSA_INS_BGEUI:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BGEUI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BGEUI instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_IMM)
            {
                LogWarn("BGEUI instruction at 0x%" PRIx64 " has non-immediate operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BGEUI instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint32_t imm = insn.operands[1].imm;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareUnsignedGreaterEqual(4, il.Register(4, s), il.Const(4, imm)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BLTUI - Branch if Less Than Unsigned Immediate
        // Operation: if (AR[s] < b4constu[r]) PC = PC + 4 + sign_extend(imm8)
        // Format: BLTUI as, b4constu, offset
        case XTENSA_INS_BLTUI:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BLTUI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BLTUI instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_IMM)
            {
                LogWarn("BLTUI instruction at 0x%" PRIx64 " has non-immediate operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BLTUI instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint32_t imm = insn.operands[1].imm;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareUnsignedLessThan(4, il.Register(4, s), il.Const(4, imm)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BBCI - Branch if Bit Clear Immediate
        // Operation: if (AR[s][bit] == 0) PC = PC + 4 + sign_extend(imm8)
        // Format: BBCI as, bit_imm, offset
        case XTENSA_INS_BBCI:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BBCI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BBCI instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_IMM)
            {
                LogWarn("BBCI instruction at 0x%" PRIx64 " has non-immediate operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BBCI instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            int32_t bit_pos = insn.operands[1].imm;
            uint64_t target_addr = insn.operands[2].target;

            if (bit_pos < 0 || bit_pos > 31)
            {
                LogWarn("BBCI instruction at 0x%" PRIx64 " has invalid bit position: %d", addr, bit_pos);
                return true;
            }

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareEqual(4,
                    il.And(4,
                        il.LogicalShiftRight(4,
                            il.Register(4, s),
                            il.Const(4, bit_pos)
                        ),
                        il.Const(4, 1)
                    ),
                    il.Const(4, 0)
                ),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BBSI - Branch if Bit Set Immediate
        // Operation: if (AR[s][bit] == 1) PC = PC + 4 + sign_extend(imm8)
        // Format: BBSI as, bit_imm, offset
        case XTENSA_INS_BBSI:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BBSI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BBSI instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_IMM)
            {
                LogWarn("BBSI instruction at 0x%" PRIx64 " has non-immediate operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BBSI instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            int32_t bit_pos = insn.operands[1].imm;
            uint64_t target_addr = insn.operands[2].target;

            if (bit_pos < 0 || bit_pos > 31)
            {
                LogWarn("BBSI instruction at 0x%" PRIx64 " has invalid bit position: %d", addr, bit_pos);
                return true;
            }

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareEqual(4,
                    il.And(4,
                        il.LogicalShiftRight(4,
                            il.Register(4, s),
                            il.Const(4, bit_pos)
                        ),
                        il.Const(4, 1)
                    ),
                    il.Const(4, 1)
                ),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BEQ - Branch if Equal
        // Operation: if (AR[s] == AR[t]) PC = PC + 4 + sign_extend(imm8)
        // Format: BEQ as, at, offset
        case XTENSA_INS_BEQ:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BEQ instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BEQ instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_REG)
            {
                LogWarn("BEQ instruction at 0x%" PRIx64 " has non-register operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BEQ instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint32_t t = insn.operands[1].reg;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareEqual(4, il.Register(4, s), il.Register(4, t)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BNE - Branch if Not Equal
        // Operation: if (AR[s] != AR[t]) PC = PC + 4 + sign_extend(imm8)
        // Format: BNE as, at, offset
        case XTENSA_INS_BNE:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BNE instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BNE instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_REG)
            {
                LogWarn("BNE instruction at 0x%" PRIx64 " has non-register operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BNE instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint32_t t = insn.operands[1].reg;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareNotEqual(4, il.Register(4, s), il.Register(4, t)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BGE - Branch if Greater Than or Equal (signed)
        // Operation: if (AR[s] >= AR[t]) PC = PC + 4 + sign_extend(imm8)
        // Format: BGE as, at, offset
        case XTENSA_INS_BGE:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BGE instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BGE instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_REG)
            {
                LogWarn("BGE instruction at 0x%" PRIx64 " has non-register operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BGE instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint32_t t = insn.operands[1].reg;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareSignedGreaterEqual(4, il.Register(4, s), il.Register(4, t)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BLT - Branch if Less Than (signed)
        // Operation: if (AR[s] < AR[t]) PC = PC + 4 + sign_extend(imm8)
        // Format: BLT as, at, offset
        case XTENSA_INS_BLT:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BLT instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BLT instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_REG)
            {
                LogWarn("BLT instruction at 0x%" PRIx64 " has non-register operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BLT instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint32_t t = insn.operands[1].reg;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareSignedLessThan(4, il.Register(4, s), il.Register(4, t)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BGEU - Branch if Greater Than or Equal Unsigned
        // Operation: if (AR[s] >= AR[t] unsigned) PC = PC + 4 + sign_extend(imm8)
        // Format: BGEU as, at, offset
        case XTENSA_INS_BGEU:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BGEU instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BGEU instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_REG)
            {
                LogWarn("BGEU instruction at 0x%" PRIx64 " has non-register operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BGEU instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint32_t t = insn.operands[1].reg;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareUnsignedGreaterEqual(4, il.Register(4, s), il.Register(4, t)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BLTU - Branch if Less Than Unsigned
        // Operation: if (AR[s] < AR[t] unsigned) PC = PC + 4 + sign_extend(imm8)
        // Format: BLTU as, at, offset
        case XTENSA_INS_BLTU:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BLTU instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BLTU instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_REG)
            {
                LogWarn("BLTU instruction at 0x%" PRIx64 " has non-register operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BLTU instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint32_t t = insn.operands[1].reg;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareUnsignedLessThan(4, il.Register(4, s), il.Register(4, t)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BANY - Branch if Any bit set
        // Operation: if ((AR[s] & AR[t]) != 0) PC = target
        // Format: BANY as, at, offset
        case XTENSA_INS_BANY:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BANY instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG ||
                insn.operands[1].type != XTENSA_OP_REG ||
                insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BANY instruction at 0x%" PRIx64 " has unexpected operand types", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint32_t t = insn.operands[1].reg;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            // Branch if (AR[s] & AR[t]) != 0
            il.AddInstruction(il.If(
                il.CompareNotEqual(4,
                    il.And(4, il.Register(4, s), il.Register(4, t)),
                    il.Const(4, 0)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BNONE - Branch if None (bitwise AND == 0)
        // Operation: if ((AR[s] & AR[t]) == 0) PC = PC + 4 + sign_extend(imm8)
        // Format: BNONE as, at, offset
        case XTENSA_INS_BNONE:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BNONE instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("BNONE instruction at 0x%" PRIx64 " has non-register operand 0", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[1].type != XTENSA_OP_REG)
            {
                LogWarn("BNONE instruction at 0x%" PRIx64 " has non-register operand 1", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BNONE instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint32_t t = insn.operands[1].reg;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            il.AddInstruction(il.If(
                il.CompareEqual(4,
                    il.And(4,
                        il.Register(4, s),
                        il.Register(4, t)
                    ),
                    il.Const(4, 0)
                ),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BALL - Branch if All bits set
        // Operation: if ((AR[s] & AR[t]) == AR[t]) PC = target
        // Format: BALL as, at, offset
        case XTENSA_INS_BALL:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BALL instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG ||
                insn.operands[1].type != XTENSA_OP_REG ||
                insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BALL instruction at 0x%" PRIx64 " has unexpected operand types", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint32_t t = insn.operands[1].reg;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            // Branch if (AR[s] & AR[t]) == AR[t]
            il.AddInstruction(il.If(
                il.CompareEqual(4,
                    il.And(4, il.Register(4, s), il.Register(4, t)),
                    il.Register(4, t)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BNALL - Branch if Not All bits set
        // Operation: if ((AR[s] & AR[t]) != AR[t]) PC = target
        // Format: BNALL as, at, offset
        case XTENSA_INS_BNALL:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BNALL instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG ||
                insn.operands[1].type != XTENSA_OP_REG ||
                insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BNALL instruction at 0x%" PRIx64 " has unexpected operand types", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint32_t t = insn.operands[1].reg;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            // Branch if (AR[s] & AR[t]) != AR[t]
            il.AddInstruction(il.If(
                il.CompareNotEqual(4,
                    il.And(4, il.Register(4, s), il.Register(4, t)),
                    il.Register(4, t)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BBC - Branch if Bit Clear (register bit position)
        // Operation: if ((AR[s] >> (AR[t] & 0x1F)) & 1 == 0) PC = target
        // Format: BBC as, at, offset
        case XTENSA_INS_BBC:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BBC instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG ||
                insn.operands[1].type != XTENSA_OP_REG ||
                insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BBC instruction at 0x%" PRIx64 " has unexpected operand types", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint32_t t = insn.operands[1].reg;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            // Branch if bit at position (AR[t] & 0x1F) in AR[s] is clear
            il.AddInstruction(il.If(
                il.CompareEqual(4,
                    il.And(4,
                        il.LogicalShiftRight(4,
                            il.Register(4, s),
                            il.And(4, il.Register(4, t), il.Const(4, 0x1F))),
                        il.Const(4, 1)),
                    il.Const(4, 0)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        // BBS - Branch if Bit Set (register bit position)
        // Operation: if ((AR[s] >> (AR[t] & 0x1F)) & 1 == 1) PC = target
        // Format: BBS as, at, offset
        case XTENSA_INS_BBS:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("BBS instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG ||
                insn.operands[1].type != XTENSA_OP_REG ||
                insn.operands[2].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("BBS instruction at 0x%" PRIx64 " has unexpected operand types", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t s = insn.operands[0].reg;
            uint32_t t = insn.operands[1].reg;
            uint64_t target_addr = insn.operands[2].target;

            LowLevelILLabel trueLabel, falseLabel;

            // Branch if bit at position (AR[t] & 0x1F) in AR[s] is set
            il.AddInstruction(il.If(
                il.CompareNotEqual(4,
                    il.And(4,
                        il.LogicalShiftRight(4,
                            il.Register(4, s),
                            il.And(4, il.Register(4, t), il.Const(4, 0x1F))),
                        il.Const(4, 1)),
                    il.Const(4, 0)),
                trueLabel,
                falseLabel
            ));

            il.MarkLabel(trueLabel);
            il.AddInstruction(il.Jump(il.ConstPointer(4, target_addr)));

            il.MarkLabel(falseLabel);
            return true;
        }

        default: break;
    }

    return false;
}
