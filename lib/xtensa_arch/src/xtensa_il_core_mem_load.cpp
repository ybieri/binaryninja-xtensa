#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 3.8.1
    Title: Load Instructions
*/

LLIL_LIFTER(core_mem_load)
{
	switch (insn.id)
	{
	case XTENSA_INS_L8UI:
	{
		if (insn.operand_count != 2)
		{
			LogWarn("L8UI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
			il.AddInstruction(il.Undefined());
			return true;
		}
		if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_MEM)
		{
			LogWarn("L8UI instruction at 0x%" PRIx64 " has unexpected operand types", addr);
			il.AddInstruction(il.Undefined());
			return true;
		}

		uint32_t dest = insn.operands[0].reg;
		uint32_t base = insn.operands[1].mem.base;
		int32_t offset = insn.operands[1].mem.offset;

		// Load 8-bit value and zero-extend to 32-bit
		il.AddInstruction(il.SetRegister(
			4, dest, il.ZeroExtend(4, il.Load(1, il.Add(4, il.Register(4, base), il.Const(4, offset))))));

		return true;
	}

	case XTENSA_INS_L16SI:
	{
		if (insn.operand_count != 2)
		{
			LogWarn("L16SI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
			il.AddInstruction(il.Undefined());
			return true;
		}
		if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_MEM)
		{
			LogWarn("L16SI instruction at 0x%" PRIx64 " has unexpected operand types", addr);
			il.AddInstruction(il.Undefined());
			return true;
		}

		uint32_t dest = insn.operands[0].reg;
		uint32_t base = insn.operands[1].mem.base;
		int32_t offset = insn.operands[1].mem.offset;

		// Load 16-bit value and sign-extend to 32-bit
		il.AddInstruction(il.SetRegister(
			4, dest, il.SignExtend(4, il.Load(2, il.Add(4, il.Register(4, base), il.Const(4, offset))))));

		return true;
	}

	case XTENSA_INS_L16UI:
	{
		if (insn.operand_count != 2)
		{
			LogWarn("L16UI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
			il.AddInstruction(il.Undefined());
			return true;
		}
		if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_MEM)
		{
			LogWarn("L16UI instruction at 0x%" PRIx64 " has unexpected operand types", addr);
			il.AddInstruction(il.Undefined());
			return true;
		}

		uint32_t dest = insn.operands[0].reg;
		uint32_t base = insn.operands[1].mem.base;
		int32_t offset = insn.operands[1].mem.offset;

		// Load 16-bit value and zero-extend to 32-bit
		il.AddInstruction(il.SetRegister(
			4, dest, il.ZeroExtend(4, il.Load(2, il.Add(4, il.Register(4, base), il.Const(4, offset))))));

		return true;
	}

	case XTENSA_INS_L32I:
	{
		if (insn.operand_count != 2)
		{
			LogWarn("L32I instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
			il.AddInstruction(il.Undefined());
			return true;
		}
		if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_MEM)
		{
			LogWarn("L32I instruction at 0x%" PRIx64 " has unexpected operand types", addr);
			il.AddInstruction(il.Undefined());
			return true;
		}

		uint32_t dest = insn.operands[0].reg;
		uint32_t base = insn.operands[1].mem.base;
		int32_t offset = insn.operands[1].mem.offset;

		il.AddInstruction(il.SetRegister(4, dest, il.Load(4, il.Add(4, il.Register(4, base), il.Const(4, offset)))));

		return true;
	}

	case XTENSA_INS_L32R:
	{
		if (insn.operand_count != 2)
		{
			LogWarn("L32R instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
			il.AddInstruction(il.Undefined());
			return true;
		}
		if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_BRANCH_TARGET)
		{
			LogWarn("L32R instruction at 0x%" PRIx64 " has unexpected operand types", addr);
			il.AddInstruction(il.Undefined());
			return true;
		}

		uint32_t dest = insn.operands[0].reg;
		uint64_t target_addr = insn.operands[1].target;

		il.AddInstruction(il.SetRegister(4, dest, il.Load(4, il.Const(4, target_addr))));

		return true;
	}

	default:
		break;
	}

	return false;
}