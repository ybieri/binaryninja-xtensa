#include "xtensa_decoder.h"


static const int32_t b4const_table[16] = {-1, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 16, 32, 64, 128, 256};


static const uint32_t b4constu_table[16] = {32768, 65536, 2, 3, 4, 5, 6, 7, 8, 10, 12, 16, 32, 64, 128, 256};


int32_t sign_extend_operand(uint32_t value, int bits)
{
	if (value & (1u << (bits - 1)))
		return (int32_t)(value | (~0u << bits));
	else
		return (int32_t)value;
}


void add_reg_operand(XtensaInstruction* insn, uint32_t reg)
{
	if (insn->operand_count >= 4)
		return;

	insn->operands[insn->operand_count].type = XTENSA_OP_REG;
	insn->operands[insn->operand_count].reg = reg & 0x0F;
	insn->operand_count++;
}


void add_fpreg_operand(XtensaInstruction* insn, uint32_t reg)
{
	if (insn->operand_count >= 4)
		return;

	insn->operands[insn->operand_count].type = XTENSA_OP_FPREG;
	insn->operands[insn->operand_count].reg = reg & 0x0F;
	insn->operand_count++;
}


void add_breg_operand(XtensaInstruction* insn, uint32_t reg)
{
	if (insn->operand_count >= 4)
		return;

	insn->operands[insn->operand_count].type = XTENSA_OP_BREG;
	insn->operands[insn->operand_count].reg = reg & 0x0F;
	insn->operand_count++;
}


void add_imm_operand(XtensaInstruction* insn, int32_t imm)
{
	if (insn->operand_count >= 4)
		return;

	insn->operands[insn->operand_count].type = XTENSA_OP_IMM;
	insn->operands[insn->operand_count].imm = imm;
	insn->operand_count++;
}


void add_mem_operand(XtensaInstruction* insn, uint32_t base_reg, int32_t offset)
{
	if (insn->operand_count >= 4)
		return;

	insn->operands[insn->operand_count].type = XTENSA_OP_MEM;
	insn->operands[insn->operand_count].mem.base = base_reg & 0x0F;
	insn->operands[insn->operand_count].mem.offset = offset;
	insn->operand_count++;
}


void add_branch_target_operand(XtensaInstruction* insn, uint64_t target)
{
	if (insn->operand_count >= 4)
		return;

	insn->operands[insn->operand_count].type = XTENSA_OP_BRANCH_TARGET;
	insn->operands[insn->operand_count].target = target;
	insn->operand_count++;
}


void add_sreg_operand(XtensaInstruction* insn, uint32_t sreg)
{
	if (insn->operand_count >= 4)
		return;

	insn->operands[insn->operand_count].type = XTENSA_OP_SREG;
	insn->operands[insn->operand_count].sreg = sreg;
	insn->operand_count++;
}


int32_t get_b4const(uint8_t encoded)
{
	return b4const_table[encoded & 0x0F];
}


uint32_t get_b4constu(uint8_t encoded)
{
	return b4constu_table[encoded & 0x0F];
}


uint64_t calc_branch_target_operand(uint64_t pc, uint32_t offset, int bits)
{
	if (bits >= 0)
	{
		int32_t signed_offset = sign_extend_operand(offset, bits);
		return pc + signed_offset + 4;
	}

	return pc + offset + 4;
}


uint64_t calc_call_target_operand(uint64_t pc, uint32_t offset, int bits)
{
	int32_t signed_offset = sign_extend_operand(offset, bits);
	uint64_t pc_word = (pc >> 2);
	uint64_t target_word = pc_word + signed_offset + 1;
	return target_word << 2;
}


uint64_t calc_l32r_target_operand(uint64_t pc, uint32_t imm)
{
	int32_t offset = (imm << 2) | 0xFFFC0000;
	uint64_t vaddr = ((pc + 3) & 0xFFFFFFFC) + offset;
	return vaddr;
}
