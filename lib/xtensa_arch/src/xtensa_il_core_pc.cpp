#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 3.8.10
    Title: Processor Control Instructions
*/

LLIL_LIFTER(core_pc)
{
	switch (insn.id)
	{
	case XTENSA_INS_RSR:
	{
		// RSR at, sr: at = SR[sr]
		// operands[0] = at (register), operands[1] = sr (special register number)
		if (insn.operand_count < 2)
			return false;

		uint32_t at = insn.operands[0].reg;
		uint32_t sr = insn.operands[1].sreg;

		// at = __rsr(sr)
		il.AddInstruction(il.Intrinsic({RegisterOrFlag::Register(at)}, ARCH_INTRINSIC_RSR, {il.Const(4, sr)}));
		return true;
	}

	case XTENSA_INS_WSR:
	{
		// WSR at, sr: SR[sr] = at
		// operands[0] = at (register), operands[1] = sr (special register number)
		if (insn.operand_count < 2)
			return false;

		uint32_t at = insn.operands[0].reg;
		uint32_t sr = insn.operands[1].sreg;

		// __wsr(sr, at)
		il.AddInstruction(il.Intrinsic({}, ARCH_INTRINSIC_WSR, {il.Const(4, sr), il.Register(4, at)}));
		return true;
	}

	case XTENSA_INS_XSR:
	{
		// XSR at, sr: temp = at; at = SR[sr]; SR[sr] = temp
		// operands[0] = at (register), operands[1] = sr (special register number)
		if (insn.operand_count < 2)
			return false;

		uint32_t at = insn.operands[0].reg;
		uint32_t sr = insn.operands[1].sreg;

		// at = __xsr(sr, at)
		il.AddInstruction(
			il.Intrinsic({RegisterOrFlag::Register(at)}, ARCH_INTRINSIC_XSR, {il.Const(4, sr), il.Register(4, at)}));
		return true;
	}

	case XTENSA_INS_ISYNC:
	{
		// ISYNC: Instruction fetch synchronize
		il.AddInstruction(il.Intrinsic({}, ARCH_INTRINSIC_ISYNC, {}));
		return true;
	}

	case XTENSA_INS_RSYNC:
	{
		// RSYNC: Register read synchronize
		il.AddInstruction(il.Intrinsic({}, ARCH_INTRINSIC_RSYNC, {}));
		return true;
	}

	case XTENSA_INS_ESYNC:
	{
		// ESYNC: Execute synchronize
		il.AddInstruction(il.Intrinsic({}, ARCH_INTRINSIC_ESYNC, {}));
		return true;
	}

	case XTENSA_INS_DSYNC:
	{
		// DSYNC: Data synchronize
		il.AddInstruction(il.Intrinsic({}, ARCH_INTRINSIC_DSYNC, {}));
		return true;
	}

	case XTENSA_INS_NOP:
	{
		il.AddInstruction(il.Nop());
		return true;
	}

	default:
		break;
	}
	return false;
}
