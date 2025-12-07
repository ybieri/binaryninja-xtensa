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


int decode_narrow(const uint8_t *data, uint64_t addr, XtensaInstruction *insn, uint32_t options)
{
    if ((options & XTENSA_OPT_CODE_DENSITY) == 0)
    {
        return 0;
    }

    uint16_t raw = data[0] | (data[1] << 8);
    insn->raw = raw;
    insn->length = 2;

    uint8_t op0 = raw & 0x0F;
    uint8_t t = (raw >> 4) & 0x0F;
    uint8_t s = (raw >> 8) & 0x0F;
    uint8_t r = (raw >> 12) & 0x0F;

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
            if (t == 0) {
                add_imm_operand(insn, -1);
            } else {
                add_imm_operand(insn, t);
            }
            return 2;
        }
        case 0x0C:
        {
            if ((t & 0x08) == 0)
            {
                insn->id = XTENSA_INS_MOVI_N;
                insn->format = XTENSA_FMT_RI7;
                add_reg_operand(insn, s);
                int32_t imm = r | ((t & 0x07) << 4);
                if (imm >= 96) {
                    imm = imm - 128;
                }
                add_imm_operand(insn, imm);
                return 2;
            }
            else
            {
                insn->format = XTENSA_FMT_RI6;
                add_reg_operand(insn, s);
                uint32_t imm6 = r | ((t & 0x03) << 4);
                uint64_t target = addr + imm6 + 4;
                add_branch_target_operand(insn, target);

                if (t & 0x04)
                {
                    insn->id = XTENSA_INS_BNEZ_N;
                }
                else
                {
                    insn->id = XTENSA_INS_BEQZ_N;
                }

                return 2;
            }
        }

        case 0x0D:
        {
            insn->format = XTENSA_FMT_RRRN;

            if (r == 0)
            {
                insn->id = XTENSA_INS_MOV_N;
                add_reg_operand(insn, t);
                add_reg_operand(insn, s);
                return 2;
            }
            else if (r == 15)
            {
                if (t == 2)
                {
                    insn->id = XTENSA_INS_BREAK_N;
                    add_imm_operand(insn, s);
                    return 2;
                }
                else if (s == 0)
                {
                    switch (t)
                    {
                        case 0:
                            insn->id = XTENSA_INS_RET_N;
                            return 2;
                        case 1:
                            insn->id = XTENSA_INS_RETW_N;
                            return 2;
                        case 3:
                            insn->id = XTENSA_INS_NOP_N;
                            return 2;
                        case 6:
                            insn->id = XTENSA_INS_ILL_N;
                            return 2;
                        default:
                            break;
                    }
                }
            }
        }
        default:
            break;
    }

    return 0;
}
