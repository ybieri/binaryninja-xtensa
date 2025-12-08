#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 3.8.3
    Title: Memory Access Ordering
*/

LLIL_LIFTER(core_mem_order)
{
	switch (insn.id)
	{
	case XTENSA_INS_MEMW:
	{
		// MEMW is a memory fence/barrier instruction
		// Use intrinsic to represent memory barrier semantics
		il.AddInstruction(il.Intrinsic({}, ARCH_INTRINSIC_MEMW, {}));
		return true;
	}

	case XTENSA_INS_EXTW:
	{
		// EXTW is an external wait/barrier instruction
		// Use intrinsic to represent barrier semantics
		il.AddInstruction(il.Intrinsic({}, ARCH_INTRINSIC_EXTW, {}));
		return true;
	}

	default:
		break;
	}
	return false;
}
