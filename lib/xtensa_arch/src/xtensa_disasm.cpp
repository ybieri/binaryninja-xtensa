#include "xtensa_disasm.h"
#include <cinttypes>
#include <cstring>
#include <cstdio>
#include <map>

using namespace BinaryNinja;


//-----------------------------------------------------------------------------
// Special Register Name Mapping
// Based on Xtensa ISA Reference Manual Table 5-176
//-----------------------------------------------------------------------------

static const std::map<uint32_t, const char*> special_register_names = {
    {0, "lbeg"},
    {1, "lend"},
    {2, "lcount"},
    {3, "sar"},
    {5, "litbase"},
    {12, "scompare1"},
    {72, "windowbase"},
    {73, "windowstart"},
    {176, "configid0"},
    {177, "epc1"},
    {178, "epc2"},
    {179, "epc3"},
    {180, "epc4"},
    {181, "epc5"},
    {182, "epc6"},
    {183, "epc7"},
    {192, "depc"},
    {194, "eps2"},
    {195, "eps3"},
    {196, "eps4"},
    {197, "eps5"},
    {198, "eps6"},
    {199, "eps7"},
    {209, "excsave1"},
    {210, "excsave2"},
    {211, "excsave3"},
    {212, "excsave4"},
    {213, "excsave5"},
    {214, "excsave6"},
    {215, "excsave7"},
    {224, "cpenable"},
    {226, "interrupt"},
    {227, "intset"},
    {228, "intclear"},
    {228, "intenable"},
    {230, "ps"},
    {231, "vecbase"},
    {232, "exccause"},
    {233, "debugcause"},
    {234, "ccount"},
    {235, "prid"},
    {236, "icount"},
    {237, "icountlevel"},
    {238, "excvaddr"},
    {244, "ccompare0"},
    {245, "ccompare1"},
    {246, "ccompare2"},
};

static const char* GetSpecialRegisterName(uint32_t sreg)
{
    auto it = special_register_names.find(sreg);
    if (it != special_register_names.end())
    {
        return it->second;
    }
    return nullptr;
}


//-----------------------------------------------------------------------------
// Operand Token Generation
//-----------------------------------------------------------------------------

void AddOperandTokens(std::vector<InstructionTextToken>& result, const XtensaOperand& op)
{
    char buf[64];

    switch (op.type)
    {
    case XTENSA_OP_REG:
        // Register: a0-a15
        snprintf(buf, sizeof(buf), "a%u", op.reg);
        result.emplace_back(RegisterToken, buf);
        break;

    case XTENSA_OP_IMM:
        // Immediate value - format based on magnitude and sign
        if (op.imm >= -16 && op.imm <= 16)
        {
            // Very small values (-16 to 16) in decimal
            snprintf(buf, sizeof(buf), "%d", op.imm);
        }
        else if (op.imm >= 0 && op.imm <= 255)
        {
            // Small positive values (17-255) in decimal
            snprintf(buf, sizeof(buf), "%d", op.imm);
        }
        else if (op.imm < 0 && op.imm >= -128)
        {
            // Small negative values in decimal
            snprintf(buf, sizeof(buf), "%d", op.imm);
        }
        else if (op.imm < 0)
        {
            // Large negative values in hex
            snprintf(buf, sizeof(buf), "-0x%x", (uint32_t)(-op.imm));
        }
        else
        {
            // Large positive values in hex
            snprintf(buf, sizeof(buf), "0x%x", (uint32_t)op.imm);
        }
        result.emplace_back(IntegerToken, buf, op.imm, 4);
        break;

    case XTENSA_OP_MEM:
        // Memory operand: a<base>, offset
        // Format: L32I a2, a1, 4  (destination, base, offset)
        snprintf(buf, sizeof(buf), "a%u", op.mem.base);
        result.emplace_back(RegisterToken, buf);
        result.emplace_back(OperandSeparatorToken, ", ");
        snprintf(buf, sizeof(buf), "%d", op.mem.offset);
        result.emplace_back(IntegerToken, buf, op.mem.offset, 4);
        break;

    case XTENSA_OP_BRANCH_TARGET:
        // Branch/call target address
        snprintf(buf, sizeof(buf), "0x%" PRIx64, op.target);
        result.emplace_back(PossibleAddressToken, buf, op.target, 4);
        break;

    case XTENSA_OP_SREG:
        // Special register - try to use name if known
        {
            const char* sr_name = GetSpecialRegisterName(op.sreg);
            if (sr_name != nullptr)
            {
                // Known special register - use name
                result.emplace_back(RegisterToken, sr_name);
            }
            else
            {
                // Unknown special register - use number
                snprintf(buf, sizeof(buf), "sr%u", op.sreg);
                result.emplace_back(RegisterToken, buf);
            }
        }
        break;

    case XTENSA_OP_FPREG:
        // Floating-point register: f0-f15
        snprintf(buf, sizeof(buf), "f%u", op.reg);
        result.emplace_back(RegisterToken, buf);
        break;

    case XTENSA_OP_BREG:
        // Boolean register: b0-b15 (used by FP compare and boolean branches)
        snprintf(buf, sizeof(buf), "b%u", op.reg);
        result.emplace_back(RegisterToken, buf);
        break;

    default:
        result.emplace_back(TextToken, "???");
        break;
    }
}
