#include <xtensa_decoder.h>


extern int32_t sign_extend_operand(uint32_t value, int bits);
extern void add_reg_operand(XtensaInstruction* insn, uint32_t reg);
extern void add_imm_operand(XtensaInstruction* insn, int32_t imm);
extern void add_mem_operand(XtensaInstruction* insn, uint32_t base_reg, int32_t offset);
extern void add_branch_target_operand(XtensaInstruction* insn, uint64_t target);
extern void add_sreg_operand(XtensaInstruction* insn, uint32_t sreg);
extern uint64_t calc_branch_target_operand(uint64_t pc, uint32_t offset, int bits);
extern uint64_t calc_call_target_operand(uint64_t pc, uint32_t offset18, int bits);
extern uint64_t calc_l32r_target_operand(uint64_t pc, uint32_t imm);
extern int32_t get_b4const(uint8_t encoded);
extern uint32_t get_b4constu(uint8_t encoded);


int decode_narrow(const uint8_t* data, uint64_t addr, XtensaInstruction* insn, uint32_t options)
{
    if ((options & XTENSA_OPT_CODE_DENSITY) == 0)
    {
        return 0;
    }

    uint16_t raw = data[0] | (data[1] << 8);
    insn->raw = raw;
    insn->length = 2;

    uint32_t op0 = raw & 0x0F;
    uint32_t t = (raw >> 4) & 0x0F;
    uint32_t s = (raw >> 8) & 0x0F;
    uint32_t r = (raw >> 12) & 0x0F;

    switch (op0)
    {
    case 0x08:
    {
        insn->id = XTENSA_INS_L32I_N;
        insn->format = XTENSA_FMT_RRRN;
        add_reg_operand(insn, t);
        add_mem_operand(insn, s, r << 2);
        return 2;
    }
    case 0x09:
    {
        insn->id = XTENSA_INS_S32I_N;
        insn->format = XTENSA_FMT_RRRN;
        add_reg_operand(insn, t);
        add_mem_operand(insn, s, r << 2);
        return 2;
    }
    case 0x0A:
    {
        insn->id = XTENSA_INS_ADD_N;
        insn->format = XTENSA_FMT_RRRN;
        add_reg_operand(insn, r);
        add_reg_operand(insn, s);
        add_reg_operand(insn, t);
        return 2;
    }
    case 0x0B:
    {
        insn->id = XTENSA_INS_ADDI_N;
        insn->format = XTENSA_FMT_RRRN;
        add_reg_operand(insn, r);
        add_reg_operand(insn, s);
        if (t == 0)
        {
            add_imm_operand(insn, -1);
        }
        else
        {
            add_imm_operand(insn, t);
        }
        return 2;
    }
    case 0x0C:
    {
        if ((t & 0b1000) == 0b0000)
        {
            insn->id = XTENSA_INS_MOVI_N;
            insn->format = XTENSA_FMT_RI7;
            add_reg_operand(insn, s);
            int32_t imm = r | ((t & 0b111) << 4);
            if (((imm >> 5) & 0b11) == 0b11)
                imm = sign_extend_operand(0b10000000 | imm, 8);
            add_imm_operand(insn, imm);
            return 2;
        }
        else if ((t & 0b1100) == 0b1100)
        {
            insn->id = XTENSA_INS_BNEZ_N;
            insn->format = XTENSA_FMT_RI6;
            add_reg_operand(insn, s);
            uint64_t target = addr + (((t & 0b11) << 4) | r) + 4;
            add_branch_target_operand(insn, target);
            return 2;
        }
        else if ((t & 0b1100) == 0b1000)
        {
            insn->id = XTENSA_INS_BEQZ_N;
            insn->format = XTENSA_FMT_RI6;
            add_reg_operand(insn, s);
            uint64_t target = addr + (((t & 0b11) << 4) | r) + 4;
            add_branch_target_operand(insn, target);
            return 2;
        }
    }

    case 0x0D:
    {
        if (r == 0)
        {
            insn->id = XTENSA_INS_MOV_N;
            insn->format = XTENSA_FMT_RRRN;
            add_reg_operand(insn, t);
            add_reg_operand(insn, s);
            return 2;
        }
        else if (r == 15 && t == 2)
        {
            insn->id = XTENSA_INS_BREAK_N;
            insn->format = XTENSA_FMT_RRRN;
            add_imm_operand(insn, s);
            return 2;
        }
        else if (r == 15 && s == 0 && t == 0)
        {
            insn->id = XTENSA_INS_RET_N;
            insn->format = XTENSA_FMT_RRRN;
            return 2;
        }
        else if (r == 15 && s == 0 && t == 1)
        {
            insn->id = XTENSA_INS_RETW_N;
            insn->format = XTENSA_FMT_RRRN;
            return 2;
        }
        else if (r == 15 && s == 0 && t == 3)
        {
            insn->id = XTENSA_INS_NOP_N;
            insn->format = XTENSA_FMT_RRRN;
            return 2;
        }
        else if (r == 15 && s == 0 && t == 6)
        {
            insn->id = XTENSA_INS_ILL_N;
            insn->format = XTENSA_FMT_RRRN;
            return 2;
        }
    }
    default:
        break;
    }

    return 0;
}
