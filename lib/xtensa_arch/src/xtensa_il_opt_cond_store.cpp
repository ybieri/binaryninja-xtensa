#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 4.3.13
    Title: Conditional Store Option
*/

LLIL_LIFTER(opt_cond_store)
{
	switch (insn.id)
	{
	case XTENSA_INS_S32C1I:
	{
		if (insn.operand_count != 2)
		{
			LogWarn("S32C1I instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
			il.AddInstruction(il.Undefined());
			return true;
		}
		if (insn.operands[0].type != XTENSA_OP_REG || insn.operands[1].type != XTENSA_OP_MEM)
		{
			LogWarn("S32C1I instruction at 0x%" PRIx64 " has unexpected operand types", addr);
			il.AddInstruction(il.Undefined());
			return true;
		}

		uint32_t t = insn.operands[0].reg;  // Value to store / destination for old value
		uint32_t base = insn.operands[1].mem.base;
		int32_t offset = insn.operands[1].mem.offset;

		// Calculate effective address: AR[s] + offset
		auto addr_expr = il.Add(4, il.Register(4, base), il.Const(4, offset));

		// Use intrinsic to represent the atomic compare-and-swap
		// __s32c1i(addr, new_value) returns old_value
		// AR[t] = __s32c1i(vAddr, AR[t])
		il.AddInstruction(il.SetRegister(4, t,
			il.Intrinsic({RegisterOrFlag::Register(t)},                // Output
				ARCH_INTRINSIC_S32C1I, {addr_expr, il.Register(4, t)}  // Inputs: addr, new_value
				)));

		return true;
	}

	default:
		break;
	}

	return false;
}