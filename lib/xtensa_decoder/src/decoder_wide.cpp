#include <xtensa_decoder.h>
#include <cstdio>


extern int32_t sign_extend_operand(uint32_t value, int bits);

extern void add_reg_operand(XtensaInstruction* insn, uint32_t reg);

extern void add_fpreg_operand(XtensaInstruction* insn, uint32_t reg);

extern void add_breg_operand(XtensaInstruction* insn, uint32_t reg);

extern void add_imm_operand(XtensaInstruction* insn, int32_t imm);

extern void add_mem_operand(XtensaInstruction* insn, uint32_t base_reg, int32_t offset);

extern void add_branch_target_operand(XtensaInstruction* insn, uint64_t target);

extern void add_sreg_operand(XtensaInstruction* insn, uint32_t sreg);

extern uint64_t calc_branch_target_operand(uint64_t pc, uint32_t offset, int bits);

extern uint64_t calc_call_target_operand(uint64_t pc, uint32_t offset, int bits);

extern uint64_t calc_l32r_target_operand(uint64_t pc, uint32_t imm);

extern int32_t get_b4const(uint8_t encoded);

extern uint32_t get_b4constu(uint8_t encoded);


static inline int _wide_op0_00(uint32_t raw, uint64_t addr, XtensaInstruction* insn, uint32_t options)
{
	uint8_t op1 = (raw >> 16) & 0x0F;
	uint8_t op2 = (raw >> 20) & 0x0F;
	uint8_t t = (raw >> 4) & 0x0F;
	uint8_t s = (raw >> 8) & 0x0F;
	uint8_t r = (raw >> 12) & 0x0F;
	uint8_t sr = (raw >> 8) & 0xFF;
	uint8_t m = (raw >> 6) & 0x03;
	uint8_t n = (raw >> 4) & 0x03;

	if (op1 == 0b0000 && op2 == 0b0000)
	{
		// CALLX format instructions
		if (r == 0b0000)
		{
			if (m == 0b10 && n == 0b10)
			{
				insn->format = XTENSA_FMT_CALLX;
				insn->id = XTENSA_INS_JX;
				add_reg_operand(insn, s);
				return 3;
			}
			if (s == 0b0000 && m == 0b10 && n == 0b00)
			{
				insn->format = XTENSA_FMT_CALLX;
				insn->id = XTENSA_INS_RET;
				return 3;
			}
			if (m == 0b11 && n == 0b00)
			{
				insn->format = XTENSA_FMT_CALLX;
				insn->id = XTENSA_INS_CALLX0;
				add_reg_operand(insn, s);
				return 3;
			}
			if (options & XTENSA_OPT_WINDOWED)
			{
				if (m == 0b11 && n == 0b01)
				{
					insn->format = XTENSA_FMT_CALLX;
					insn->id = XTENSA_INS_CALLX4;
					add_reg_operand(insn, s);
					return 3;
				}
				if (m == 0b11 && n == 0b10)
				{
					insn->format = XTENSA_FMT_CALLX;
					insn->id = XTENSA_INS_CALLX8;
					add_reg_operand(insn, s);
					return 3;
				}
				if (m == 0b11 && n == 0b11)
				{
					insn->format = XTENSA_FMT_CALLX;
					insn->id = XTENSA_INS_CALLX12;
					add_reg_operand(insn, s);
					return 3;
				}
				if (s == 0b0000 && m == 0b10 && n == 0b01)
				{
					insn->format = XTENSA_FMT_CALLX;
					insn->id = XTENSA_INS_RETW;
					return 3;
				}
			}
		}

		// SYNC and NOP instructions
		if (r == 0b0010 && s == 0b0000)
		{
			XtensaInsnId sync_ins_id = XTENSA_INS_INVALID;

			if (t == 0b0000)
				sync_ins_id = XTENSA_INS_ISYNC;
			else if (t == 0b0001)
				sync_ins_id = XTENSA_INS_RSYNC;
			else if (t == 0b0010)
				sync_ins_id = XTENSA_INS_ESYNC;
			else if (t == 0b0011)
				sync_ins_id = XTENSA_INS_DSYNC;
			else if (t == 0b1100)
				sync_ins_id = XTENSA_INS_MEMW;
			else if (t == 0b1101)
				sync_ins_id = XTENSA_INS_EXTW;
			else if (t == 0b1111)
				sync_ins_id = XTENSA_INS_NOP;

			if (sync_ins_id != XTENSA_INS_INVALID)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = sync_ins_id;
				return 3;
			}
		}

		if (options & XTENSA_OPT_WINDOWED)
		{
			// MOVSP
			if (r == 0b0001)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_MOVSP;
				add_reg_operand(insn, t);
				add_reg_operand(insn, s);
				return 3;
			}
			// RFW instructions
			if (r == 0b0011 && t == 0b0000)
			{
				XtensaInsnId rfw_ins_id = XTENSA_INS_INVALID;

				if (s == 0b0100)
					rfw_ins_id = XTENSA_INS_RFWO;
				else if (s == 0b0101)
					rfw_ins_id = XTENSA_INS_RFWU;

				if (rfw_ins_id != XTENSA_INS_INVALID)
				{
					insn->format = XTENSA_FMT_RRR;
					insn->id = rfw_ins_id;
					return 3;
				}
			}
		}

		if (options & XTENSA_OPT_EXCEPTION2)
		{
			if (r == 0b0010 && s == 0b0000 && t == 0b1000)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_EXCW;
				return 3;
			}
			if (r == 0b0101 && s == 0b0000)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_SYSCALL;
				add_imm_operand(insn, t);
				return 3;
			}
			if (r == 0b0011 && s == 0b000 && t == 0b0000)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_RFE;
				return 3;
			}
			if (r == 0b0011 && s == 0b0010 && t == 0b0000)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_RFDE;
				return 3;
			}
		}
	}

	if (op1 == 0b0000)
	{
		if (op2 == 0b0001)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_AND;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b0010)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_OR;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b0011)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_XOR;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b0100)
		{
			if (r == 0b0000 && t == 0b0000)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_SSR;
				add_reg_operand(insn, s);
				return 3;
			}
			if (r == 0b0001 && t == 0b0000)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_SSL;
				add_reg_operand(insn, s);
				return 3;
			}
			if (r == 0b0010 && t == 0b0000)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_SSA8L;
				add_reg_operand(insn, s);
				return 3;
			}
			if (r == 0b0011 && t == 0b0000)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_SSA8B;
				add_reg_operand(insn, s);
				return 3;
			}
			if (r == 0b0100 && ((t & 0b1110) == 0b0000))
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_SSAI;
				add_imm_operand(insn, s | ((t & 1) << 4));
				return 3;
			}
			if (options & XTENSA_OPT_WINDOWED)
			{
				if (r == 0b1000 && s == 0b0000)
				{
					insn->format = XTENSA_FMT_RRR;
					insn->id = XTENSA_INS_ROTW;
					add_imm_operand(insn, sign_extend_operand(t, 4));
					return 3;
				}
			}
			if (options & XTENSA_OPT_MISC)
			{
				if (r == 0b1110)
				{
					insn->format = XTENSA_FMT_RRR;
					insn->id = XTENSA_INS_NSA;
					add_reg_operand(insn, t);
					add_reg_operand(insn, s);
					return 3;
				}
				if (r == 0b1111)
				{
					insn->format = XTENSA_FMT_RRR;
					insn->id = XTENSA_INS_NSAU;
					add_reg_operand(insn, t);
					add_reg_operand(insn, s);
					return 3;
				}
			}
		}
		if (op2 == 0b0110 && s == 0b0000)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_NEG;
			add_reg_operand(insn, r);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b0110 && s == 0b0001)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_ABS;
			add_reg_operand(insn, r);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b1000)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_ADD;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b1001)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_ADDX2;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b1010)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_ADDX4;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b1011)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_ADDX8;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b1100)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_SUB;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b1101)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_SUBX2;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b1110)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_SUBX4;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b1111)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_SUBX8;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
	}

	if (op1 == 0b0001)
	{
		if (op2 == 0b0000 || op2 == 0b0001)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_SLLI;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_imm_operand(insn, t | ((op2 & 1) << 4));
			return 3;
		}
		if (op2 == 0b0010 || op2 == 0b0011)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_SRAI;
			add_reg_operand(insn, r);
			add_reg_operand(insn, t);
			add_imm_operand(insn, s | ((op2 & 1) << 4));
			return 3;
		}
		if (op2 == 0b0100)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_SRLI;
			add_reg_operand(insn, r);
			add_reg_operand(insn, t);
			add_imm_operand(insn, s);
			return 3;
		}
		if (op2 == 0b0110)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_XSR;
			add_reg_operand(insn, t);
			add_sreg_operand(insn, sr);
			return 3;
		}
		if (op2 == 0b1000)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_SRC;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b1001 && s == 0b0000)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_SRL;
			add_reg_operand(insn, r);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b1010 && t == 0b0000)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_SLL;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			return 3;
		}
		if (op2 == 0b1011 && s == 0b0000)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_SRA;
			add_reg_operand(insn, r);
			add_reg_operand(insn, t);
			return 3;
		}
		if (options & XTENSA_OPT_MUL16)
		{
			if (op2 == 0b1100)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_MUL16U;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_reg_operand(insn, t);
				return 3;
			}
			if (op2 == 0b1101)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_MUL16S;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_reg_operand(insn, t);
				return 3;
			}
		}
	}

	if (op1 == 0b0010)
	{
		if (options & XTENSA_OPT_MUL32)
		{
			if (op2 == 0b1000)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_MULL;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_reg_operand(insn, t);
				return 3;
			}
			if (op2 == 0b1010)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_MULUH;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_reg_operand(insn, t);
				return 3;
			}
			if (op2 == 0b1011)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_MULSH;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_reg_operand(insn, t);
				return 3;
			}
		}

		if (options & XTENSA_OPT_DIV32)
		{
			if (op2 == 0b1101)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_QUOS;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_reg_operand(insn, t);
				return 3;
			}
			if (op2 == 0b1100)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_QUOU;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_reg_operand(insn, t);
				return 3;
			}
			if (op2 == 0b1111)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_REMS;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_reg_operand(insn, t);
				return 3;
			}
			if (op2 == 0b1110)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_REMU;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_reg_operand(insn, t);
				return 3;
			}
		}
	}

	if (op1 == 0b0011)
	{
		if (op2 == 0b0000)
		{
			insn->format = XTENSA_FMT_RSR;
			insn->id = XTENSA_INS_RSR;
			add_reg_operand(insn, t);
			add_sreg_operand(insn, sr);
			return 3;
		}
		if (op2 == 0b0001)
		{
			insn->format = XTENSA_FMT_RSR;
			insn->id = XTENSA_INS_WSR;
			add_reg_operand(insn, t);
			add_sreg_operand(insn, sr);
			return 3;
		}
		if (options & XTENSA_OPT_MISC)
		{
			if (op2 == 0b0011)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_CLAMPS;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_imm_operand(insn, t + 7);
				return 3;
			}
			if (op2 == 0b0111)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_MAX;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_reg_operand(insn, t);
				return 3;
			}
			if (op2 == 0b0101)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_MAXU;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_reg_operand(insn, t);
				return 3;
			}
			if (op2 == 0b0110)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_MIN;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_reg_operand(insn, t);
				return 3;
			}
			if (op2 == 0b0100)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_MINU;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_reg_operand(insn, t);
				return 3;
			}
			if (op2 == 0b0010)
			{
				insn->format = XTENSA_FMT_RRR;
				insn->id = XTENSA_INS_SEXT;
				add_reg_operand(insn, r);
				add_reg_operand(insn, s);
				add_imm_operand(insn, t + 7);
				return 3;
			}
		}
		if (op2 == 0b1000)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_MOVEQZ;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b1001)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_MOVNEZ;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b1010)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_MOVLTZ;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		if (op2 == 0b1011)
		{
			insn->format = XTENSA_FMT_RRR;
			insn->id = XTENSA_INS_MOVGEZ;
			add_reg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
	}

	if (op1 == 0b0100)
	{
		insn->format = XTENSA_FMT_RRR;
		insn->id = XTENSA_INS_EXTUI;
		add_reg_operand(insn, r);
		add_reg_operand(insn, t);
		uint8_t shiftimm = ((op1 & 1) << 4) | s;
		add_imm_operand(insn, shiftimm);
		uint8_t maskimm = op2 + 1;
		add_imm_operand(insn, maskimm);
		return 3;
	}

	if ((op1 & 0b1110) == 0b1010 && options & XTENSA_OPT_DEPBITS)
	{
		insn->format = XTENSA_FMT_RRR;
		insn->id = XTENSA_INS_DEPBITS;
		add_reg_operand(insn, t);
		add_reg_operand(insn, s);
		uint8_t shiftimm = ((op1 & 1) << 4) | op2;
		add_imm_operand(insn, shiftimm);
		uint8_t maskimm = r + 1;
		add_imm_operand(insn, maskimm);
		return 3;
	}

	if (op1 == 0b1010)
	{
		insn->format = XTENSA_FMT_RRR;
		switch (op2)
		{
		case 0b0000:
		{
			insn->id = XTENSA_INS_ADD_S;
			add_fpreg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_fpreg_operand(insn, t);
			return 3;
		}
		case 0b0001:
		{
			insn->id = XTENSA_INS_SUB_S;
			add_fpreg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_fpreg_operand(insn, t);
			return 3;
		}
		case 0b0010:
		{
			insn->id = XTENSA_INS_MUL_S;
			add_fpreg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_fpreg_operand(insn, t);
			return 3;
		}
		case 0b0100:
		{
			insn->id = XTENSA_INS_MADD_S;
			add_fpreg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_fpreg_operand(insn, t);
			return 3;
		}
		case 0b0101:
		{
			insn->id = XTENSA_INS_MSUB_S;
			add_fpreg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_fpreg_operand(insn, t);
			return 3;
		}
		case 0b0110:
		{
			insn->id = XTENSA_INS_MADDN_S;
			add_fpreg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_fpreg_operand(insn, t);
			return 3;
		}
		case 0b0111:
		{
			insn->id = XTENSA_INS_DIVN_S;
			add_fpreg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_fpreg_operand(insn, t);
			return 3;
		}
		case 0b1000:
		{
			insn->id = XTENSA_INS_ROUND_S;
			add_reg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_imm_operand(insn, t);
			return 3;
		}
		case 0b1001:
		{
			insn->id = XTENSA_INS_TRUNC_S;
			add_reg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_imm_operand(insn, t);
			return 3;
		}
		case 0b1010:
		{
			insn->id = XTENSA_INS_FLOOR_S;
			add_reg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_imm_operand(insn, t);
			return 3;
		}
		case 0b1011:
		{
			insn->id = XTENSA_INS_CEIL_S;
			add_reg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_imm_operand(insn, t);
			return 3;
		}
		case 0b1100:
		{
			insn->id = XTENSA_INS_FLOAT_S;
			add_fpreg_operand(insn, r);
			add_reg_operand(insn, s);
			add_imm_operand(insn, t);
			return 3;
		}
		case 0b1101:
		{
			insn->id = XTENSA_INS_UFLOAT_S;
			add_fpreg_operand(insn, r);
			add_reg_operand(insn, s);
			add_imm_operand(insn, t);
			return 3;
		}
		case 0b1110:
		{
			insn->id = XTENSA_INS_UTRUNC_S;
			add_reg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_imm_operand(insn, t);
			return 3;
		}
		case 0b1111:
		{
			switch (t)
			{
			case 0b0000:
			{
				insn->id = XTENSA_INS_MOV_S;
				add_fpreg_operand(insn, r);
				add_fpreg_operand(insn, s);
				return 3;
			}
			case 0b0001:
			{
				insn->id = XTENSA_INS_ABS_S;
				add_fpreg_operand(insn, r);
				add_fpreg_operand(insn, s);
				return 3;
			}
			case 0b0011:
			{
				insn->id = XTENSA_INS_CONST_S;
				add_fpreg_operand(insn, r);
				add_imm_operand(insn, s);
				return 3;
			}
			case 0b0100:
			{
				insn->id = XTENSA_INS_RFR;
				add_reg_operand(insn, r);
				add_fpreg_operand(insn, s);
				return 3;
			}
			case 0b0101:
			{
				insn->id = XTENSA_INS_WFR;
				add_fpreg_operand(insn, r);
				add_reg_operand(insn, s);
				return 3;
			}
			case 0b0110:
			{
				insn->id = XTENSA_INS_NEG_S;
				add_fpreg_operand(insn, r);
				add_fpreg_operand(insn, s);
				return 3;
			}
			case 0b0111:
			{
				insn->id = XTENSA_INS_DIV0_S;
				add_fpreg_operand(insn, r);
				add_fpreg_operand(insn, s);
				return 3;
			}
			case 0b1000:
			{
				insn->id = XTENSA_INS_RECIP0_S;
				add_fpreg_operand(insn, r);
				add_fpreg_operand(insn, s);
				return 3;
			}
			case 0b1001:
			{
				insn->id = XTENSA_INS_SQRT0_S;
				add_fpreg_operand(insn, r);
				add_fpreg_operand(insn, s);
				return 3;
			}
			case 0b1010:
			{
				insn->id = XTENSA_INS_RSQRT0_S;
				add_fpreg_operand(insn, r);
				add_fpreg_operand(insn, s);
				return 3;
			}
			case 0b1011:
			{
				insn->id = XTENSA_INS_NEXP01_S;
				add_fpreg_operand(insn, r);
				add_fpreg_operand(insn, s);
				return 3;
			}
			case 0b1100:
			{
				insn->id = XTENSA_INS_MKSADJ_S;
				add_fpreg_operand(insn, r);
				add_fpreg_operand(insn, s);
				return 3;
			}
			case 0b1101:
			{
				insn->id = XTENSA_INS_MKDADJ_S;
				add_fpreg_operand(insn, r);
				add_fpreg_operand(insn, s);
				return 3;
			}
			case 0b1110:
			{
				insn->id = XTENSA_INS_ADDEXP_S;
				add_fpreg_operand(insn, r);
				add_fpreg_operand(insn, s);
				return 3;
			}
			case 0b1111:
			{
				insn->id = XTENSA_INS_ADDEXPM_S;
				add_fpreg_operand(insn, r);
				add_fpreg_operand(insn, s);
				return 3;
			}
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
	}

	if (op1 == 0b1011)
	{
		insn->format = XTENSA_FMT_RRR;
		switch (op2)
		{
		case 0b0001:
		{
			insn->id = XTENSA_INS_UN_S;
			add_breg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_fpreg_operand(insn, t);
			return 3;
		}
		case 0b0010:
		{
			insn->id = XTENSA_INS_OEQ_S;
			add_breg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_fpreg_operand(insn, t);
			return 3;
		}
		case 0b0011:
		{
			insn->id = XTENSA_INS_UEQ_S;
			add_breg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_fpreg_operand(insn, t);
			return 3;
		}
		case 0b0100:
		{
			insn->id = XTENSA_INS_OLT_S;
			add_breg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_fpreg_operand(insn, t);
			return 3;
		}
		case 0b0101:
		{
			insn->id = XTENSA_INS_ULT_S;
			add_breg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_fpreg_operand(insn, t);
			return 3;
		}
		case 0b0110:
		{
			insn->id = XTENSA_INS_OLE_S;
			add_breg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_fpreg_operand(insn, t);
			return 3;
		}
		case 0b0111:
		{
			insn->id = XTENSA_INS_ULE_S;
			add_breg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_fpreg_operand(insn, t);
			return 3;
		}
		case 0b1000:
		{
			insn->id = XTENSA_INS_MOVEQZ_S;
			add_fpreg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		case 0b1001:
		{
			insn->id = XTENSA_INS_MOVNEZ_S;
			add_fpreg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		case 0b1010:
		{
			insn->id = XTENSA_INS_MOVLTZ_S;
			add_fpreg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		case 0b1011:
		{
			insn->id = XTENSA_INS_MOVGEZ_S;
			add_fpreg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		case 0b1100:
		{
			insn->id = XTENSA_INS_MOVF_S;
			add_fpreg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		case 0b1101:
		{
			insn->id = XTENSA_INS_MOVT_S;
			add_fpreg_operand(insn, r);
			add_fpreg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		default:
			break;
		}
	}

	if (op1 == 0b1000)
	{
		insn->format = XTENSA_FMT_RRR;
		switch (op2)
		{
		case 0b0000:
		{
			insn->id = XTENSA_INS_LSX;
			add_fpreg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		case 0b0001:
		{
			insn->id = XTENSA_INS_LSXU;
			add_fpreg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		case 0b0100:
		{
			insn->id = XTENSA_INS_SSX;
			add_fpreg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		case 0b0101:
		{
			insn->id = XTENSA_INS_SSXU;
			add_fpreg_operand(insn, r);
			add_reg_operand(insn, s);
			add_reg_operand(insn, t);
			return 3;
		}
		default:
			break;
		}
	}

	return 0;
}


static inline int _wide_op0_01(uint32_t raw, uint64_t addr, XtensaInstruction* insn, uint32_t options)
{
	uint8_t t = (raw >> 4) & 0x0F;
	uint16_t imm = (raw >> 8) & 0xFFFF;
	insn->format = XTENSA_FMT_RI16;
	insn->id = XTENSA_INS_L32R;
	add_reg_operand(insn, t);
	uint64_t target = calc_l32r_target_operand(addr, imm);
	add_branch_target_operand(insn, target);
	return 3;
}


static inline int _wide_op0_02(uint32_t raw, uint64_t addr, XtensaInstruction* insn, uint32_t options)
{
	uint8_t t = (raw >> 4) & 0x0F;
	uint8_t s = (raw >> 8) & 0x0F;
	uint8_t r = (raw >> 12) & 0x0F;
	uint8_t imm8 = (raw >> 16) & 0xFF;

	insn->format = XTENSA_FMT_RRI8;

	if (r == 0b0000)
	{
		insn->id = XTENSA_INS_L8UI;
		add_reg_operand(insn, t);
		add_mem_operand(insn, s, imm8);
		return 3;
	}
	if (r == 0b0001)
	{
		insn->id = XTENSA_INS_L16UI;
		add_reg_operand(insn, t);
		add_mem_operand(insn, s, imm8 << 1);
		return 3;
	}
	if (r == 0b1001)
	{
		insn->id = XTENSA_INS_L16SI;
		add_reg_operand(insn, t);
		add_mem_operand(insn, s, imm8 << 1);
		return 3;
	}
	if (r == 0b0010)
	{
		insn->id = XTENSA_INS_L32I;
		add_reg_operand(insn, t);
		add_mem_operand(insn, s, imm8 << 2);
		return 3;
	}
	if (r == 0b0100)
	{
		insn->id = XTENSA_INS_S8I;
		add_reg_operand(insn, t);
		add_mem_operand(insn, s, imm8);
		return 3;
	}
	if (r == 0b0101)
	{
		insn->id = XTENSA_INS_S16I;
		add_reg_operand(insn, t);
		add_mem_operand(insn, s, imm8 << 1);
		return 3;
	}
	if (r == 0b0110)
	{
		insn->id = XTENSA_INS_S32I;
		add_reg_operand(insn, t);
		add_mem_operand(insn, s, imm8 << 2);
		return 3;
	}
	if (r == 0b1010)
	{
		insn->id = XTENSA_INS_MOVI;
		add_reg_operand(insn, t);
		int16_t imm12 = (s << 8) | imm8;
		add_imm_operand(insn, sign_extend_operand(imm12, 12));
		return 3;
	}
	if (r == 0b1100)
	{
		insn->id = XTENSA_INS_ADDI;
		add_reg_operand(insn, t);
		add_reg_operand(insn, s);
		add_imm_operand(insn, sign_extend_operand(imm8, 8));
		return 3;
	}
	if (r == 0b1101)
	{
		insn->id = XTENSA_INS_ADDMI;
		add_reg_operand(insn, t);
		add_reg_operand(insn, s);
		add_imm_operand(insn, sign_extend_operand(imm8, 8) << 8);
		return 3;
	}
	if (options & XTENSA_OPT_SYNC)
	{
		if (r == 0b1011)
		{
			insn->id = XTENSA_INS_L32AI;
			add_reg_operand(insn, t);
			add_mem_operand(insn, s, imm8 << 2);
			return 3;
		}
		if (r == 0b1111)
		{
			insn->id = XTENSA_INS_S32RI;
			add_reg_operand(insn, t);
			add_mem_operand(insn, s, imm8 << 2);
			return 3;
		}
	}
	if (options & XTENSA_OPT_COND_STORE)
	{
		if (r == 0b1110)
		{
			insn->id = XTENSA_INS_S32C1I;
			add_reg_operand(insn, t);
			add_mem_operand(insn, s, imm8 << 1);
			return 3;
		}
	}

	return 0;
}


static inline int _wide_op0_03(uint32_t raw, uint64_t addr, XtensaInstruction* insn, uint32_t options)
{
	uint8_t t = (raw >> 4) & 0x0F;
	uint8_t s = (raw >> 8) & 0x0F;
	uint8_t r = (raw >> 12) & 0x0F;
	uint8_t imm8 = (raw >> 16) & 0xFF;

	insn->format = XTENSA_FMT_RRI8;

	int32_t offset = imm8 << 2;

	switch (r)
	{
	case 0x0:
	{
		insn->id = XTENSA_INS_LSI;
		add_fpreg_operand(insn, t);
		add_mem_operand(insn, s, offset);
		return 3;
	}
	case 0x4:
	{
		insn->id = XTENSA_INS_SSI;
		add_fpreg_operand(insn, t);
		add_mem_operand(insn, s, offset);
		return 3;
	}
	case 0x8:
	{
		insn->id = XTENSA_INS_LSIU;
		add_fpreg_operand(insn, t);
		add_mem_operand(insn, s, offset);
		return 3;
	}
	case 0xC:
	{
		insn->id = XTENSA_INS_SSIU;
		add_fpreg_operand(insn, t);
		add_mem_operand(insn, s, offset);
		return 3;
	}
	default:
		break;
	}

	return 0;
}


static inline int _wide_op0_04(uint32_t raw, uint64_t addr, XtensaInstruction* insn, uint32_t options)
{
	if (options & XTENSA_OPT_MAC16)
	{
		insn->format = XTENSA_FMT_RRR;
		insn->id = XTENSA_INS_MAC16;

		uint8_t t = (raw >> 4) & 0x0F;
		uint8_t s = (raw >> 8) & 0x0F;
		uint8_t r = (raw >> 12) & 0x0F;

		add_reg_operand(insn, r);
		add_reg_operand(insn, s);
		add_reg_operand(insn, t);

		return 3;
	}

	return 0;
}


static inline int _wide_op0_05(uint32_t raw, uint64_t addr, XtensaInstruction* insn, uint32_t options)
{
	uint8_t n = (raw >> 4) & 0b11;
	uint32_t offset18 = (raw >> 6) & 0x3FFFF;
	uint64_t target = calc_call_target_operand(addr, offset18, 18);

	if (n == 0b00)
	{
		insn->format = XTENSA_FMT_CALL;
		insn->id = XTENSA_INS_CALL0;
		add_branch_target_operand(insn, target);
		return 3;
	}

	if (options & XTENSA_OPT_WINDOWED)
	{
		if (n == 0b01)
		{
			insn->format = XTENSA_FMT_CALL;
			insn->id = XTENSA_INS_CALL4;
			add_branch_target_operand(insn, target);
			return 3;
		}
		if (n == 0b10)
		{
			insn->format = XTENSA_FMT_CALL;
			insn->id = XTENSA_INS_CALL8;
			add_branch_target_operand(insn, target);
			return 3;
		}
		if (n == 0b11)
		{
			insn->format = XTENSA_FMT_CALL;
			insn->id = XTENSA_INS_CALL12;
			add_branch_target_operand(insn, target);
			return 3;
		}
	}

	return 0;
}


static inline int _wide_op0_06(uint32_t raw, uint64_t addr, XtensaInstruction* insn, uint32_t options)
{
	uint8_t mn = (raw >> 4) & 0x0F;
	uint8_t n = (raw >> 4) & 0b11;
	uint8_t m = (raw >> 6) & 0b11;

	if (n == 0b00)
	{
		uint32_t offset18 = (raw >> 6) & 0x3FFFF;
		insn->format = XTENSA_FMT_CALL;
		insn->id = XTENSA_INS_J;
		uint64_t target = calc_branch_target_operand(addr, offset18, 18);
		add_branch_target_operand(insn, target);
		return 3;
	}

	{
		uint8_t s = (raw >> 8) & 0x0F;
		uint32_t imm12 = (raw >> 12) & 0xFFF;
		uint64_t target = calc_branch_target_operand(addr, imm12, 12);

		if (mn == 0b0001)
		{
			insn->format = XTENSA_FMT_BRI12;
			insn->id = XTENSA_INS_BEQZ;
			add_reg_operand(insn, s);
			add_branch_target_operand(insn, target);
			return 3;
		}
		if (mn == 0b0101)
		{
			insn->format = XTENSA_FMT_BRI12;
			insn->id = XTENSA_INS_BNEZ;
			add_reg_operand(insn, s);
			add_branch_target_operand(insn, target);
			return 3;
		}
		if (mn == 0b1001)
		{
			insn->format = XTENSA_FMT_BRI12;
			insn->id = XTENSA_INS_BLTZ;
			add_reg_operand(insn, s);
			add_branch_target_operand(insn, target);
			return 3;
		}
		if (mn == 0b1101)
		{
			insn->format = XTENSA_FMT_BRI12;
			insn->id = XTENSA_INS_BGEZ;
			add_reg_operand(insn, s);
			add_branch_target_operand(insn, target);
			return 3;
		}
		if (mn == 0b0011 && options & XTENSA_OPT_WINDOWED)
		{
			insn->format = XTENSA_FMT_BRI12;
			insn->id = XTENSA_INS_ENTRY;
			add_reg_operand(insn, s);
			add_imm_operand(insn, imm12 << 3);
			return 3;
		}
	}

	// BRI8 format instructions
	uint8_t t = (raw >> 4) & 0x0F;
	uint8_t s = (raw >> 8) & 0x0F;
	uint8_t r = (raw >> 12) & 0x0F;
	uint8_t imm8 = (raw >> 16) & 0xFF;

	if (t == 0b0111)
	{
		if (options & XTENSA_OPT_BOOLEAN)
		{
			if (r == 0)
			{
				insn->format = XTENSA_FMT_BRI8;
				insn->id = XTENSA_INS_BF;
				add_breg_operand(insn, s);
				add_branch_target_operand(insn, calc_branch_target_operand(addr, imm8, 8));
				return 3;
			}
			if (r == 1)
			{
				insn->format = XTENSA_FMT_BRI8;
				insn->id = XTENSA_INS_BT;
				add_breg_operand(insn, s);
				add_branch_target_operand(insn, calc_branch_target_operand(addr, imm8, 8));
				return 3;
			}
		}
		if (options & XTENSA_OPT_LOOP)
		{
			if (r == 8)
			{
				insn->format = XTENSA_FMT_BRI8;
				insn->id = XTENSA_INS_LOOP;
				add_reg_operand(insn, s);
				add_branch_target_operand(insn, calc_branch_target_operand(addr, imm8, -1));
				return 3;
			}
			if (r == 9)
			{
				insn->format = XTENSA_FMT_BRI8;
				insn->id = XTENSA_INS_LOOPNEZ;
				add_reg_operand(insn, s);
				add_branch_target_operand(insn, calc_branch_target_operand(addr, imm8, -1));
				return 3;
			}
			if (r == 10)
			{
				insn->format = XTENSA_FMT_BRI8;
				insn->id = XTENSA_INS_LOOPGTZ;
				add_reg_operand(insn, s);
				add_branch_target_operand(insn, calc_branch_target_operand(addr, imm8, -1));
				return 3;
			}
		}
	}

	uint64_t branch_target = calc_branch_target_operand(addr, imm8, 8);

	switch (t)
	{
	case 0b0010:
	{
		insn->format = XTENSA_FMT_BRI8;
		insn->id = XTENSA_INS_BEQI;
		add_reg_operand(insn, s);
		add_imm_operand(insn, get_b4const(r));
		add_branch_target_operand(insn, branch_target);
		return 3;
	}
	case 0b0110:
	{
		insn->format = XTENSA_FMT_BRI8;
		insn->id = XTENSA_INS_BNEI;
		add_reg_operand(insn, s);
		add_imm_operand(insn, get_b4const(r));
		add_branch_target_operand(insn, branch_target);
		return 3;
	}
	case 0b1010:
	{
		insn->format = XTENSA_FMT_BRI8;
		insn->id = XTENSA_INS_BLTI;
		add_reg_operand(insn, s);
		add_imm_operand(insn, get_b4const(r));
		add_branch_target_operand(insn, branch_target);
		return 3;
	}
	case 0b1011:
	{
		insn->format = XTENSA_FMT_BRI8;
		insn->id = XTENSA_INS_BLTUI;
		add_reg_operand(insn, s);
		add_imm_operand(insn, get_b4constu(r));
		add_branch_target_operand(insn, branch_target);
		return 3;
	}
	case 0b1110:
	{
		insn->format = XTENSA_FMT_BRI8;
		insn->id = XTENSA_INS_BGEI;
		add_reg_operand(insn, s);
		add_imm_operand(insn, get_b4const(r));
		add_branch_target_operand(insn, branch_target);
		return 3;
	}
	case 0b1111:
	{
		insn->format = XTENSA_FMT_BRI8;
		insn->id = XTENSA_INS_BGEUI;
		add_reg_operand(insn, s);
		add_imm_operand(insn, get_b4constu(r));
		add_branch_target_operand(insn, branch_target);
		return 3;
	}
	default:
		break;
	}

	return 0;
}


static inline int _wide_op0_07(uint32_t raw, uint64_t addr, XtensaInstruction* insn, uint32_t options)
{
	insn->format = XTENSA_FMT_BRI8;

	uint8_t t = (raw >> 4) & 0x0F;
	uint8_t s = (raw >> 8) & 0x0F;
	uint8_t r = (raw >> 12) & 0x0F;
	uint8_t imm8 = (raw >> 16) & 0xFF;
	uint64_t target = calc_branch_target_operand(addr, imm8, 8);

	switch (r)
	{
	case 0x0:
	{
		insn->id = XTENSA_INS_BNONE;
		add_reg_operand(insn, s);
		add_reg_operand(insn, t);
		add_branch_target_operand(insn, target);
		return 3;
	}
	case 0x1:
	{
		insn->id = XTENSA_INS_BEQ;
		add_reg_operand(insn, s);
		add_reg_operand(insn, t);
		add_branch_target_operand(insn, target);
		return 3;
	}
	case 0x2:
	{
		insn->id = XTENSA_INS_BLT;
		add_reg_operand(insn, s);
		add_reg_operand(insn, t);
		add_branch_target_operand(insn, target);
		return 3;
	}
	case 0x3:
	{
		insn->id = XTENSA_INS_BLTU;
		add_reg_operand(insn, s);
		add_reg_operand(insn, t);
		add_branch_target_operand(insn, target);
		return 3;
	}
	case 0x4:
	{
		insn->id = XTENSA_INS_BALL;
		add_reg_operand(insn, s);
		add_reg_operand(insn, t);
		add_branch_target_operand(insn, target);
		return 3;
	}
	case 0x5:
	{
		insn->id = XTENSA_INS_BBC;
		add_reg_operand(insn, s);
		add_reg_operand(insn, t);
		add_branch_target_operand(insn, target);
		return 3;
	}
	case 0x6:
	case 0x7:
	{
		insn->id = XTENSA_INS_BBCI;
		add_reg_operand(insn, s);
		add_imm_operand(insn, ((r & 0x1) << 4) | (t & 0x0F));
		add_branch_target_operand(insn, target);
		return 3;
	}
	case 0x8:
	{
		insn->id = XTENSA_INS_BANY;
		add_reg_operand(insn, s);
		add_reg_operand(insn, t);
		add_branch_target_operand(insn, target);
		return 3;
	}
	case 0x9:
	{
		insn->id = XTENSA_INS_BNE;
		add_reg_operand(insn, s);
		add_reg_operand(insn, t);
		add_branch_target_operand(insn, target);
		return 3;
	}
	case 0xA:
	{
		insn->id = XTENSA_INS_BGE;
		add_reg_operand(insn, s);
		add_reg_operand(insn, t);
		add_branch_target_operand(insn, target);
		return 3;
	}
	case 0xB:
	{
		insn->id = XTENSA_INS_BGEU;
		add_reg_operand(insn, s);
		add_reg_operand(insn, t);
		add_branch_target_operand(insn, target);
		return 3;
	}
	case 0xC:
	{
		insn->id = XTENSA_INS_BNALL;
		add_reg_operand(insn, s);
		add_reg_operand(insn, t);
		add_branch_target_operand(insn, target);
		return 3;
	}
	case 0xD:
	{
		insn->id = XTENSA_INS_BBS;
		add_reg_operand(insn, s);
		add_reg_operand(insn, t);
		add_branch_target_operand(insn, target);
		return 3;
	}
	case 0xE:
	case 0xF:
	{
		insn->id = XTENSA_INS_BBSI;
		add_reg_operand(insn, s);
		// Bit position: b[4:0] = r[0] << 4 | t[3:0]
		add_imm_operand(insn, ((r & 0x1) << 4) | (t & 0x0F));
		add_branch_target_operand(insn, target);
		return 3;
	}
	default:
		break;
	}

	return 0;
}


static inline uint32_t read_insn_bytes(const uint8_t* data, int length)
{
	uint32_t insn = 0;
	for (int i = 0; i < length; i++)
	{
		insn |= ((uint32_t)data[i]) << (i * 8);
	}
	return insn;
}


int decode_wide(const uint8_t* data, uint64_t addr, XtensaInstruction* insn, uint32_t options)
{
	(void)options;  // Reserved for future option checking

	uint32_t raw = read_insn_bytes(data, 3);
	insn->raw = raw;
	insn->length = 3;

	uint8_t op0 = raw & 0x0F;

	if (raw == 0x000000)
	{
		insn->format = XTENSA_FMT_CALLX;
		insn->id = XTENSA_INS_ILL;
		return 3;
	}

	switch (op0)
	{
	case 0x00:
		return _wide_op0_00(raw, addr, insn, options);
	case 0x01:
		return _wide_op0_01(raw, addr, insn, options);
	case 0x02:
		return _wide_op0_02(raw, addr, insn, options);
	case 0x03:
		return _wide_op0_03(raw, addr, insn, options);
	case 0x04:
		return _wide_op0_04(raw, addr, insn, options);
	case 0x05:
		return _wide_op0_05(raw, addr, insn, options);
	case 0x06:
		return _wide_op0_06(raw, addr, insn, options);
	case 0x07:
		return _wide_op0_07(raw, addr, insn, options);
	default:
		break;
	}

	return 0;
}