#include "xtensa_decoder.h"

const char* xtensa_mnemonic(XtensaInsnId id)
{
	switch (id)
	{
	// Arithmetic & Logical
	case XTENSA_INS_ADD:
		return "add";
	case XTENSA_INS_ADDI:
		return "addi";
	case XTENSA_INS_ADDMI:
		return "addmi";
	case XTENSA_INS_ADDX2:
		return "addx2";
	case XTENSA_INS_ADDX4:
		return "addx4";
	case XTENSA_INS_ADDX8:
		return "addx8";
	case XTENSA_INS_SUB:
		return "sub";
	case XTENSA_INS_SUBX2:
		return "subx2";
	case XTENSA_INS_SUBX4:
		return "subx4";
	case XTENSA_INS_SUBX8:
		return "subx8";
	case XTENSA_INS_AND:
		return "and";
	case XTENSA_INS_OR:
		return "or";
	case XTENSA_INS_XOR:
		return "xor";
	case XTENSA_INS_NEG:
		return "neg";
	case XTENSA_INS_ABS:
		return "abs";

	// RFW
	case XTENSA_INS_RFWO:
		return "rfwo";
	case XTENSA_INS_RFWU:
		return "rfwu";

	// Move operations
	case XTENSA_INS_MOV:
		return "mov";
	case XTENSA_INS_MOVI:
		return "movi";
	case XTENSA_INS_MOVEQZ:
		return "moveqz";
	case XTENSA_INS_MOVNEZ:
		return "movnez";
	case XTENSA_INS_MOVLTZ:
		return "movltz";
	case XTENSA_INS_MOVGEZ:
		return "movgez";

	// Shift operations
	case XTENSA_INS_SLL:
		return "sll";
	case XTENSA_INS_SLLI:
		return "slli";
	case XTENSA_INS_SRL:
		return "srl";
	case XTENSA_INS_SRLI:
		return "srli";
	case XTENSA_INS_SRA:
		return "sra";
	case XTENSA_INS_SRAI:
		return "srai";
	case XTENSA_INS_SRC:
		return "src";
	case XTENSA_INS_SSL:
		return "ssl";
	case XTENSA_INS_SSR:
		return "ssr";
	case XTENSA_INS_SSA8L:
		return "ssa8l";
	case XTENSA_INS_SSA8B:
		return "ssa8b";
	case XTENSA_INS_SSAI:
		return "ssai";

	// Multiply operations
	case XTENSA_INS_MULL:
		return "mull";
	case XTENSA_INS_MULUH:
		return "muluh";
	case XTENSA_INS_MULSH:
		return "mulsh";
	case XTENSA_INS_MUL16U:
		return "mul16u";
	case XTENSA_INS_MUL16S:
		return "mul16s";

	// Divide and remainder operations
	case XTENSA_INS_QUOU:
		return "quou";
	case XTENSA_INS_QUOS:
		return "quos";
	case XTENSA_INS_REMU:
		return "remu";
	case XTENSA_INS_REMS:
		return "rems";

	// Miscellaneous operations
	case XTENSA_INS_NSA:
		return "nsa";
	case XTENSA_INS_NSAU:
		return "nsau";
	case XTENSA_INS_MIN:
		return "min";
	case XTENSA_INS_MAX:
		return "max";
	case XTENSA_INS_MINU:
		return "minu";
	case XTENSA_INS_MAXU:
		return "maxu";

	// Load operations
	case XTENSA_INS_L8UI:
		return "l8ui";
	case XTENSA_INS_L16UI:
		return "l16ui";
	case XTENSA_INS_L16SI:
		return "l16si";
	case XTENSA_INS_L32I:
		return "l32i";
	case XTENSA_INS_L32R:
		return "l32r";

	// Store operations
	case XTENSA_INS_S8I:
		return "s8i";
	case XTENSA_INS_S16I:
		return "s16i";
	case XTENSA_INS_S32I:
		return "s32i";

	// Atomic operations
	case XTENSA_INS_L32AI:
		return "l32ai";
	case XTENSA_INS_S32C1I:
		return "s32c1i";
	case XTENSA_INS_S32RI:
		return "s32ri";

	// MAC16 operations
	case XTENSA_INS_MAC16:
		return "mac16";

	// Floating-Point Arithmetic
	case XTENSA_INS_ADD_S:
		return "add.s";
	case XTENSA_INS_SUB_S:
		return "sub.s";
	case XTENSA_INS_MUL_S:
		return "mul.s";
	case XTENSA_INS_MADD_S:
		return "madd.s";
	case XTENSA_INS_MSUB_S:
		return "msub.s";
	case XTENSA_INS_MADDN_S:
		return "maddn.s";
	case XTENSA_INS_DIVN_S:
		return "divn.s";

	// Floating-Point Conversion
	case XTENSA_INS_ROUND_S:
		return "round.s";
	case XTENSA_INS_TRUNC_S:
		return "trunc.s";
	case XTENSA_INS_FLOOR_S:
		return "floor.s";
	case XTENSA_INS_CEIL_S:
		return "ceil.s";
	case XTENSA_INS_FLOAT_S:
		return "float.s";
	case XTENSA_INS_UFLOAT_S:
		return "ufloat.s";
	case XTENSA_INS_UTRUNC_S:
		return "utrunc.s";

	// Floating-Point Unary Operations
	case XTENSA_INS_MOV_S:
		return "mov.s";
	case XTENSA_INS_ABS_S:
		return "abs.s";
	case XTENSA_INS_NEG_S:
		return "neg.s";
	case XTENSA_INS_RFR:
		return "rfr";
	case XTENSA_INS_WFR:
		return "wfr";
	case XTENSA_INS_CONST_S:
		return "const.s";
	case XTENSA_INS_DIV0_S:
		return "div0.s";
	case XTENSA_INS_RECIP0_S:
		return "recip0.s";
	case XTENSA_INS_SQRT0_S:
		return "sqrt0.s";
	case XTENSA_INS_RSQRT0_S:
		return "rsqrt0.s";
	case XTENSA_INS_NEXP01_S:
		return "nexp01.s";
	case XTENSA_INS_MKSADJ_S:
		return "mksadj.s";
	case XTENSA_INS_MKDADJ_S:
		return "mkdadj.s";
	case XTENSA_INS_ADDEXP_S:
		return "addexp.s";
	case XTENSA_INS_ADDEXPM_S:
		return "addexpm.s";

	// Floating-Point Compare
	case XTENSA_INS_UN_S:
		return "un.s";
	case XTENSA_INS_OEQ_S:
		return "oeq.s";
	case XTENSA_INS_UEQ_S:
		return "ueq.s";
	case XTENSA_INS_OLT_S:
		return "olt.s";
	case XTENSA_INS_ULT_S:
		return "ult.s";
	case XTENSA_INS_OLE_S:
		return "ole.s";
	case XTENSA_INS_ULE_S:
		return "ule.s";

	// Floating-Point Conditional Move
	case XTENSA_INS_MOVEQZ_S:
		return "moveqz.s";
	case XTENSA_INS_MOVNEZ_S:
		return "movnez.s";
	case XTENSA_INS_MOVLTZ_S:
		return "movltz.s";
	case XTENSA_INS_MOVGEZ_S:
		return "movgez.s";
	case XTENSA_INS_MOVF_S:
		return "movf.s";
	case XTENSA_INS_MOVT_S:
		return "movt.s";

	// Floating-Point Load/Store Immediate
	case XTENSA_INS_LSI:
		return "lsi";
	case XTENSA_INS_SSI:
		return "ssi";
	case XTENSA_INS_LSIU:
		return "lsiu";
	case XTENSA_INS_SSIU:
		return "ssiu";

	// Floating-Point Load/Store Indexed
	case XTENSA_INS_LSX:
		return "lsx";
	case XTENSA_INS_LSXU:
		return "lsxu";
	case XTENSA_INS_SSX:
		return "ssx";
	case XTENSA_INS_SSXU:
		return "ssxu";

	// Unconditional branches
	case XTENSA_INS_J:
		return "j";
	case XTENSA_INS_JX:
		return "jx";

	// Zero-Overhead Loops
	case XTENSA_INS_LOOP:
		return "loop";
	case XTENSA_INS_LOOPNEZ:
		return "loopnez";
	case XTENSA_INS_LOOPGTZ:
		return "loopgtz";

	// Conditional branches
	case XTENSA_INS_BEQ:
		return "beq";
	case XTENSA_INS_BNE:
		return "bne";
	case XTENSA_INS_BLT:
		return "blt";
	case XTENSA_INS_BLTU:
		return "bltu";
	case XTENSA_INS_BGE:
		return "bge";
	case XTENSA_INS_BGEU:
		return "bgeu";
	case XTENSA_INS_BEQZ:
		return "beqz";
	case XTENSA_INS_BNEZ:
		return "bnez";
	case XTENSA_INS_BLTZ:
		return "bltz";
	case XTENSA_INS_BGEZ:
		return "bgez";

	// Bit test branches
	case XTENSA_INS_BALL:
		return "ball";
	case XTENSA_INS_BANY:
		return "bany";
	case XTENSA_INS_BBC:
		return "bbc";
	case XTENSA_INS_BBS:
		return "bbs";
	case XTENSA_INS_BBCI:
		return "bbci";
	case XTENSA_INS_BBSI:
		return "bbsi";
	case XTENSA_INS_BNONE:
		return "bnone";
	case XTENSA_INS_BNALL:
		return "bnall";

	// Immediate compare branches
	case XTENSA_INS_BEQI:
		return "beqi";
	case XTENSA_INS_BNEI:
		return "bnei";
	case XTENSA_INS_BLTI:
		return "blti";
	case XTENSA_INS_BLTUI:
		return "bltui";
	case XTENSA_INS_BGEI:
		return "bgei";
	case XTENSA_INS_BGEUI:
		return "bgeui";

	// Boolean branches (Boolean Option)
	case XTENSA_INS_BF:
		return "bf";
	case XTENSA_INS_BT:
		return "bt";

	// Call operations (windowed ABI)
	case XTENSA_INS_CALL0:
		return "call0";
	case XTENSA_INS_CALL4:
		return "call4";
	case XTENSA_INS_CALL8:
		return "call8";
	case XTENSA_INS_CALL12:
		return "call12";
	case XTENSA_INS_CALLX0:
		return "callx0";
	case XTENSA_INS_CALLX4:
		return "callx4";
	case XTENSA_INS_CALLX8:
		return "callx8";
	case XTENSA_INS_CALLX12:
		return "callx12";

	// Return operations
	case XTENSA_INS_RET:
		return "ret";
	case XTENSA_INS_RETW:
		return "retw";

	// Windowed register operations
	case XTENSA_INS_ENTRY:
		return "entry";
	case XTENSA_INS_MOVSP:
		return "movsp";
	case XTENSA_INS_ROTW:
		return "rotw";

	// Special register access
	case XTENSA_INS_RSR:
		return "rsr";
	case XTENSA_INS_WSR:
		return "wsr";
	case XTENSA_INS_XSR:
		return "xsr";

	// System/Control
	case XTENSA_INS_NOP:
		return "nop";
	case XTENSA_INS_RFE:
		return "rfe";
	case XTENSA_INS_RFDE:
		return "rfde";
	case XTENSA_INS_EXCW:
		return "excw";
	case XTENSA_INS_SYSCALL:
		return "syscall";
	case XTENSA_INS_SIMCALL:
		return "simcall";
	case XTENSA_INS_BREAK:
		return "break";
	case XTENSA_INS_MEMW:
		return "memw";
	case XTENSA_INS_EXTW:
		return "extw";
	case XTENSA_INS_ISYNC:
		return "isync";
	case XTENSA_INS_RSYNC:
		return "rsync";
	case XTENSA_INS_ESYNC:
		return "esync";
	case XTENSA_INS_DSYNC:
		return "dsync";

	// Code Density - 16-bit narrow instructions
	case XTENSA_INS_ADD_N:
		return "add.n";
	case XTENSA_INS_ADDI_N:
		return "addi.n";
	case XTENSA_INS_MOV_N:
		return "mov.n";
	case XTENSA_INS_MOVI_N:
		return "movi.n";
	case XTENSA_INS_L32I_N:
		return "l32i.n";
	case XTENSA_INS_S32I_N:
		return "s32i.n";
	case XTENSA_INS_BEQZ_N:
		return "beqz.n";
	case XTENSA_INS_BNEZ_N:
		return "bnez.n";
	case XTENSA_INS_RET_N:
		return "ret.n";
	case XTENSA_INS_RETW_N:
		return "retw.n";
	case XTENSA_INS_BREAK_N:
		return "break.n";
	case XTENSA_INS_NOP_N:
		return "nop.n";
	case XTENSA_INS_ILL_N:
		return "ill.n";

	// Additional instructions
	case XTENSA_INS_EXTUI:
		return "extui";
	case XTENSA_INS_SEXT:
		return "sext";
	case XTENSA_INS_CLAMPS:
		return "clamps";
	case XTENSA_INS_DEPBITS:
		return "depbits";

	case XTENSA_INS_ILL:
		return "ill";

	case XTENSA_INS_MAX_VALUE:
	default:
		return "unknown";
	}
}
