#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 3.8.2
    Title: Store Instructions
*/

LLIL_LIFTER(core_mem_store)
{
	switch (insn.id)
	{
	case XTENSA_INS_S8I:
	{
		if (insn.operand_count != 2)
		{
			LogWarn("S8I instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
			il.AddInstruction(il.Undefined());
			return true;
		}
		if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_MEM)
		{
			LogWarn("S8I instruction at 0x%" PRIx64 " has unexpected operand types", addr);
			il.AddInstruction(il.Undefined());
			return true;
		}

		uint32_t src = insn.operands[0].reg;
		uint32_t base = insn.operands[1].mem.base;
		int32_t offset = insn.operands[1].mem.offset;

		il.AddInstruction(
			il.Store(1, il.Add(4, il.Register(4, base), il.Const(4, offset)), il.LowPart(1, il.Register(4, src))));

		return true;
	}

	case XTENSA_INS_S16I:
	{
		if (insn.operand_count != 2)
		{
			LogWarn("S16I instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
			il.AddInstruction(il.Undefined());
			return true;
		}
		if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_MEM)
		{
			LogWarn("S16I instruction at 0x%" PRIx64 " has unexpected operand types", addr);
			il.AddInstruction(il.Undefined());
			return true;
		}

		uint32_t src = insn.operands[0].reg;
		uint32_t base = insn.operands[1].mem.base;
		int32_t offset = insn.operands[1].mem.offset;

		il.AddInstruction(
			il.Store(2, il.Add(4, il.Register(4, base), il.Const(4, offset)), il.LowPart(2, il.Register(4, src))));

		return true;
	}

	case XTENSA_INS_S32I:
	{
		if (insn.operand_count != 2)
		{
			LogWarn("S32I instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
			il.AddInstruction(il.Undefined());
			return true;
		}
		if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_MEM)
		{
			LogWarn("S32I instruction at 0x%" PRIx64 " has unexpected operand types", addr);
			il.AddInstruction(il.Undefined());
			return true;
		}

		uint32_t src = insn.operands[0].reg;
		uint32_t base = insn.operands[1].mem.base;
		int32_t offset = insn.operands[1].mem.offset;

		il.AddInstruction(il.Store(4, il.Add(4, il.Register(4, base), il.Const(4, offset)), il.Register(4, src)));

		return true;
	}

	case XTENSA_INS_SSI:
	{
		if (insn.operand_count != 2)
		{
			LogWarn("SSI instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
			il.AddInstruction(il.Undefined());
			return true;
		}
		if (insn.operands[0].type != XTENSA_OP_FPREG || insn.operands[1].type != XTENSA_OP_MEM)
		{
			LogWarn("SSI instruction at 0x%" PRIx64 " has unexpected operand types (expected FPREG, got %d)", addr,
				insn.operands[0].type);
			il.AddInstruction(il.Undefined());
			return true;
		}

		uint32_t src = insn.operands[0].reg;  // Floating-point register ft (stored in reg field)
		uint32_t base = insn.operands[1].mem.base;
		int32_t offset = insn.operands[1].mem.offset;

		// SSI offset is already scaled by 4 in decoder (imm8 << 2)
		// Store 32-bit floating-point value
		// Note: Since Binary Ninja's architecture doesn't have explicit FP register types,
		// we treat the floating-point register as a general register for LLIL purposes
		il.AddInstruction(il.Store(4, il.Add(4, il.Register(4, base), il.Const(4, offset)), il.Register(4, src)));

		return true;
	}

	default:
		break;
	}

	return false;
}