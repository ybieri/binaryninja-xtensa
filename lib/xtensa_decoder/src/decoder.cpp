#include <xtensa_decoder.h>
#include <cstring>


extern int decode_wide(const uint8_t *data, uint64_t addr, XtensaInstruction *insn, uint32_t options);

extern int decode_narrow(const uint8_t *data, uint64_t addr, XtensaInstruction *insn, uint32_t options);


int xtensa_decode(const uint8_t *data, size_t maxLen, uint64_t addr, XtensaInstruction *insn, uint32_t options)
{
    if (data == nullptr || maxLen < 2 || insn == nullptr)
        return 0;

    *insn = XtensaInstruction();

    uint8_t first_byte = data[0];
    uint8_t op0 = first_byte & 0x0F;

    int ret = 0;

    if (op0 >= 8)
        ret = decode_narrow(data, addr, insn, options);
    else if (maxLen >= 3)
        ret = decode_wide(data, addr, insn, options);

    if (ret != 0)
    {
        const char *mnemonic = xtensa_mnemonic(insn->id);
        insn->mnemonic = mnemonic;
    }

    return ret;
}
