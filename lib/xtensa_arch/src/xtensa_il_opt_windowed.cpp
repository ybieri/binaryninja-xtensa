#include "xtensa_il.h"
#include "xtensa_arch.h"
#include "binaryninjacore.h"


// Helper to set ILTransparentCopy attribute on the last added instruction.
// This tells Binary Ninja's analysis to follow through these copies for proper
// data flow analysis (used for register windowing operations).
//
// Reference: binaryninja-api/arch/arm64/il.cpp:1236-1241
static void SetTransparentCopyOnLastInstruction(LowLevelILFunction& il)
{
    size_t instrId = il.GetInstructionCount() - 1;
    ExprId expr = il.GetIndexForInstruction(instrId);
    il.SetExprAttributes(expr, ILTransparentCopy);
}


// Helper for windowed calls (CALLn/CALLXn where n > 0)
// Implements register rotation emulation with ILTransparentCopy attribute
// for proper data flow analysis through the windowed register rotation.
//
// The ILTransparentCopy attribute signals to Binary Ninja's use-def analysis
// that these register copies are architectural artifacts (window rotation)
// and should be followed through transparently for parameter inference.
//
// For CALLX variants, dest_expr must be pre-evaluated to a temp before calling this,
// as the register shift may overwrite the source register.
static void LiftWindowedCall(LowLevelILFunction& il, ExprId dest_expr, int n)
{
    uint32_t temps[16];

    // 1. Save first n registers (a0..an-1) to temps
    //    Mark with ILTransparentCopy for transparent data flow
    for (int i = 0; i < n; i++) {
        temps[i] = LLIL_TEMP(i);
        il.AddInstruction(il.SetRegister(4, temps[i], il.Register(4, REG_A0 + i)));
        SetTransparentCopyOnLastInstruction(il);
    }

    // 2. Shift registers down: a[i] = a[i+n]
    //    This maps caller's argument registers to callee's perspective
    //    Mark with ILTransparentCopy for transparent data flow
    for (int i = 0; i < 16 - n; i++) {
        il.AddInstruction(il.SetRegister(4, REG_A0 + i, il.Register(4, REG_A0 + i + n)));
        SetTransparentCopyOnLastInstruction(il);
    }

    // 3. Emit the actual call (NOT marked with ILTransparentCopy)
    il.AddInstruction(il.Call(dest_expr));

    // 4. Restore registers up: a[i+n] = a[i]
    //    This maps callee's return to caller's perspective
    //    Mark with ILTransparentCopy for transparent data flow
    for (int i = 0; i < 16 - n; i++) {
        il.AddInstruction(il.SetRegister(4, REG_A0 + i + n, il.Register(4, REG_A0 + i)));
        SetTransparentCopyOnLastInstruction(il);
    }

    // 5. Restore first n registers from temps
    //    Mark with ILTransparentCopy for transparent data flow
    for (int i = 0; i < n; i++) {
        il.AddInstruction(il.SetRegister(4, REG_A0 + i, il.Register(4, temps[i])));
        SetTransparentCopyOnLastInstruction(il);
    }
}


LLIL_LIFTER(opt_windowed)
{
    switch (insn.id)
    {
        case XTENSA_INS_MOVSP:
        {
            // TODO: need implementation
            il.AddInstruction(il.Unimplemented());
            return true;
        }

        case XTENSA_INS_ENTRY:
        {
            if (insn.operand_count != 2)
            {
                LogWarn("ENTRY instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_IMM)
            {
                LogWarn("ENTRY instruction at 0x%" PRIx64 " has unexpected operand types", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t stack_reg = insn.operands[0].reg;
            int32_t framesize = insn.operands[1].imm;

            il.AddInstruction(il.SetRegister(4, stack_reg,
                il.Sub(4,
                    il.Register(4, stack_reg),
                    il.Const(4, framesize)
                )
            ));

            return true;
        }

        case XTENSA_INS_CALL4:
        {
            if (insn.operand_count != 1)
            {
                LogWarn("CALL4 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("CALL4 instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                break;
            }

            uint64_t target_addr = insn.operands[0].target;

            // Windowed Call with ILTransparentCopy rotation (window increment = 4)
            LiftWindowedCall(il, il.ConstPointer(4, target_addr), 4);

            return true;
        }

        case XTENSA_INS_CALL8:
        {
            if (insn.operand_count != 1)
            {
                LogWarn("CALL8 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("CALL8 instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                break;
            }

            uint64_t target_addr = insn.operands[0].target;

            // Windowed Call with ILTransparentCopy rotation (window increment = 8)
            LiftWindowedCall(il, il.ConstPointer(4, target_addr), 8);

            return true;
        }

        case XTENSA_INS_CALL12:
        {
            if (insn.operand_count != 1)
            {
                LogWarn("CALL12 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_BRANCH_TARGET)
            {
                LogWarn("CALL12 instruction at 0x%" PRIx64 " has unexpected operand type", addr);
                il.AddInstruction(il.Undefined());
                break;
            }

            uint64_t target_addr = insn.operands[0].target;

            // Windowed Call with ILTransparentCopy rotation (window increment = 12)
            LiftWindowedCall(il, il.ConstPointer(4, target_addr), 12);

            return true;
        }

        case XTENSA_INS_CALLX4:
        {
            if (insn.operand_count != 1)
            {
                LogWarn("CALLX4 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("CALLX4 instruction at 0x%" PRIx64 " has non-register operand", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t src = insn.operands[0].reg;

            // Save call target to temp before window rotation (src reg may be overwritten)
            uint32_t call_target_temp = LLIL_TEMP(16);
            il.AddInstruction(il.SetRegister(4, call_target_temp, il.Register(4, src)));
            SetTransparentCopyOnLastInstruction(il);

            // Windowed Call with ILTransparentCopy rotation (window increment = 4)
            LiftWindowedCall(il, il.Register(4, call_target_temp), 4);

            return true;
        }

        case XTENSA_INS_CALLX8:
        {
            if (insn.operand_count != 1) {
                LogWarn("CALLX8 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG) {
                LogWarn("CALLX8 instruction at 0x%" PRIx64 " has non-register operand", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t src = insn.operands[0].reg;

            // Save call target to temp before window rotation (src reg may be overwritten)
            uint32_t call_target_temp = LLIL_TEMP(16);
            il.AddInstruction(il.SetRegister(4, call_target_temp, il.Register(4, src)));
            SetTransparentCopyOnLastInstruction(il);

            // Windowed Call with ILTransparentCopy rotation (window increment = 8)
            LiftWindowedCall(il, il.Register(4, call_target_temp), 8);

            return true;
        }

        case XTENSA_INS_CALLX12:
        {
            if (insn.operand_count != 1)
            {
                LogWarn("CALLX12 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                return true;
            }
            if (insn.operands[0].type != XTENSA_OP_REG)
            {
                LogWarn("CALLX12 instruction at 0x%" PRIx64 " has non-register operand", addr);
                il.AddInstruction(il.Undefined());
                return true;
            }

            uint32_t src = insn.operands[0].reg;

            // Save call target to temp before window rotation (src reg may be overwritten)
            uint32_t call_target_temp = LLIL_TEMP(16);
            il.AddInstruction(il.SetRegister(4, call_target_temp, il.Register(4, src)));
            SetTransparentCopyOnLastInstruction(il);

            // Windowed Call with ILTransparentCopy rotation (window increment = 12)
            LiftWindowedCall(il, il.Register(4, call_target_temp), 12);

            return true;
        }

        case XTENSA_INS_RETW:
        {
            // Simplified: just return from A0, ignoring window rotation for now
            il.AddInstruction(il.Return(
                il.Register(4, REG_A0)
            ));

            return true;
        }

        default: break;
    }

    return false;
}
