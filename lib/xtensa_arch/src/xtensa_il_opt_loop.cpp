#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 4.3.2
    Title: Loop Option
*/

LLIL_LIFTER(opt_loop)
{
	switch (insn.id)
	{
	case XTENSA_INS_LOOP:
	{
		if (insn.operand_count != 2)
		{
			LogWarn("LOOP instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
			il.AddInstruction(il.Undefined());
			return true;
		}
		if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_BRANCH_TARGET)
		{
			LogWarn("LOOP instruction at 0x%" PRIx64 " has unexpected operand types", addr);
			il.AddInstruction(il.Undefined());
			return true;
		}

		uint32_t count_reg = insn.operands[0].reg;
		uint64_t loop_end = insn.operands[1].target;
		uint64_t loop_begin = addr + insn.length;  // Next instruction

		// LCOUNT = AR[s] - 1
		il.AddInstruction(il.SetRegister(4, REG_LCOUNT, il.Sub(4, il.Register(4, count_reg), il.Const(4, 1))));

		// LBEG = nextPC
		il.AddInstruction(il.SetRegister(4, REG_LBEG, il.Const(4, loop_begin)));

		// LEND = label
		il.AddInstruction(il.SetRegister(4, REG_LEND, il.Const(4, loop_end)));

		return true;
	}

	case XTENSA_INS_LOOPGTZ:
	{
		if (insn.operand_count != 2)
		{
			LogWarn("LOOPGTZ instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
			il.AddInstruction(il.Undefined());
			return true;
		}
		if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_BRANCH_TARGET)
		{
			LogWarn("LOOPGTZ instruction at 0x%" PRIx64 " has unexpected operand types", addr);
			il.AddInstruction(il.Undefined());
			return true;
		}

		uint32_t count_reg = insn.operands[0].reg;
		uint64_t loop_end = insn.operands[1].target;
		uint64_t loop_begin = addr + insn.length;

		LowLevelILLabel loopLabel, skipLabel;

		// if (AR[s] > 0) goto loopLabel else goto skipLabel
		il.AddInstruction(
			il.If(il.CompareSignedGreaterThan(4, il.Register(4, count_reg), il.Const(4, 0)), loopLabel, skipLabel));

		// Loop setup
		il.MarkLabel(loopLabel);
		il.AddInstruction(il.SetRegister(4, REG_LCOUNT, il.Sub(4, il.Register(4, count_reg), il.Const(4, 1))));
		il.AddInstruction(il.SetRegister(4, REG_LBEG, il.Const(4, loop_begin)));
		il.AddInstruction(il.SetRegister(4, REG_LEND, il.Const(4, loop_end)));

		// Skip loop (jump to loop_end)
		il.MarkLabel(skipLabel);
		il.AddInstruction(il.Jump(il.ConstPointer(4, loop_end)));

		return true;
	}

	case XTENSA_INS_LOOPNEZ:
	{
		if (insn.operand_count != 2)
		{
			LogWarn("LOOPNEZ instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
			il.AddInstruction(il.Undefined());
			return true;
		}
		if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_BRANCH_TARGET)
		{
			LogWarn("LOOPNEZ instruction at 0x%" PRIx64 " has unexpected operand types", addr);
			il.AddInstruction(il.Undefined());
			return true;
		}

		uint32_t count_reg = insn.operands[0].reg;
		uint64_t loop_end = insn.operands[1].target;
		uint64_t loop_begin = addr + insn.length;

		LowLevelILLabel loopLabel, skipLabel;

		// if (AR[s] != 0) goto loopLabel else goto skipLabel
		il.AddInstruction(
			il.If(il.CompareNotEqual(4, il.Register(4, count_reg), il.Const(4, 0)), loopLabel, skipLabel));

		// Loop setup
		il.MarkLabel(loopLabel);
		il.AddInstruction(il.SetRegister(4, REG_LCOUNT, il.Sub(4, il.Register(4, count_reg), il.Const(4, 1))));
		il.AddInstruction(il.SetRegister(4, REG_LBEG, il.Const(4, loop_begin)));
		il.AddInstruction(il.SetRegister(4, REG_LEND, il.Const(4, loop_end)));

		// Skip loop (jump to loop_end)
		il.MarkLabel(skipLabel);
		il.AddInstruction(il.Jump(il.ConstPointer(4, loop_end)));

		return true;
	}

	default:
		break;
	}

	return false;
}