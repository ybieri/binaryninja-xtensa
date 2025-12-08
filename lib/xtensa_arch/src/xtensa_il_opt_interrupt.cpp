#include "xtensa_il.h"
#include "xtensa_arch.h"

/*
    Document: references/Xtensa-Summary.pdf
    Chapter: 4.4.5
    Title: Interrupt Option
*/

LLIL_LIFTER(opt_interrupt)
{
	switch (insn.id)
	{
		// TODO: XTENSA_INS_RSIL unimplemented in decoder
		// case XTENSA_INS_RSIL: { ... }

		// TODO: XTENSA_INS_WAITI unimplemented in decoder
		// case XTENSA_INS_WAITI: { ... }

		// TODO: XTENSA_INS_RFI unimplemented in decoder
		// case XTENSA_INS_RFI: { ... }

	default:
		break;
	}

	return false;
}