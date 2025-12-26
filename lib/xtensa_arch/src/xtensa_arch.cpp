#include <stdio.h>
#include <string.h>
#include <vector>

#include "binaryninjaapi.h"
#include "lowlevelilinstruction.h"

#include "xtensa_arch.h"
#include "xtensa_disasm.h"
#include "xtensa_decoder.h"
#include "xtensa_il.h"

using namespace BinaryNinja;
using namespace std;


//-----------------------------------------------------------------------------
// Architecture - Constructor
//-----------------------------------------------------------------------------

XtensaArchitecture::XtensaArchitecture(const char* name, BNEndianness endian, uint32_t options) :
    Architecture(name), m_endian(endian), m_addressSize(4), m_options(options) {}


//-----------------------------------------------------------------------------
// Architecture - Core Informs
//-----------------------------------------------------------------------------

BNEndianness XtensaArchitecture::GetEndianness() const
{
    return m_endian;
}

size_t XtensaArchitecture::GetAddressSize() const
{
    return m_addressSize;
}

size_t XtensaArchitecture::GetDefaultIntegerSize() const
{
    return 4;
}

size_t XtensaArchitecture::GetInstructionAlignment() const
{
    return 1;  // Xtensa has variable-length instructions
}

size_t XtensaArchitecture::GetMaxInstructionLength() const
{
    return 3;  // Max 24-bit instructions
}


//-----------------------------------------------------------------------------
// Architecture - Core Disassembly & Liftings
//-----------------------------------------------------------------------------

static inline bool instr_is_call(XtensaInsnId id)
{
    return (id >= XTENSA_INS_CALL0 && id <= XTENSA_INS_CALLX12);
}

static inline bool instr_is_return(XtensaInsnId id)
{
    return (
        id == XTENSA_INS_RET ||
        id == XTENSA_INS_RETW ||
        id == XTENSA_INS_RET_N ||
        id == XTENSA_INS_RETW_N
    );
}

static inline bool instr_is_branch(XtensaInsnId id)
{
    return (
        id == XTENSA_INS_J ||
        id == XTENSA_INS_JX ||
        id == XTENSA_INS_BF ||
        id == XTENSA_INS_BT ||
        id == XTENSA_INS_BEQZ_N ||
        id == XTENSA_INS_BNEZ_N ||
        (id >= XTENSA_INS_BEQ && id <= XTENSA_INS_BGEUI)
    );
}

static inline bool instr_is_conditional(XtensaInsnId id)
{
    return (
        id == XTENSA_INS_BF ||
        id == XTENSA_INS_BT ||
        id == XTENSA_INS_BEQZ_N ||
        id == XTENSA_INS_BNEZ_N ||
        (id >= XTENSA_INS_BEQ && id <= XTENSA_INS_BGEUI)
    );
}

bool XtensaArchitecture::GetInstructionInfo(const uint8_t* data, uint64_t addr, size_t maxLen, InstructionInfo& result)
{
    XtensaInstruction insn;

    // Decode instruction
    int decoded_len = xtensa_decode(data, maxLen, addr, &insn, m_options);
    if (decoded_len == 0)
        return false;

    // Set instruction length
    result.length = insn.length;

    // Analyze control flow using helper functions
    if (instr_is_call(insn.id))
    {
        // Call instructions (CALL0/4/8/12, CALLX0/4/8/12)
        if (insn.operands[0].type == XTENSA_OP_BRANCH_TARGET)
        {
            // Direct call
            result.AddBranch(CallDestination, insn.operands[0].target);
        }
        else
        {
            // Indirect call
            result.AddBranch(CallDestination);
        }
    }
    else if (instr_is_return(insn.id))
    {
        // Return instructions (RET, RETW, RFE, etc.)
        result.AddBranch(FunctionReturn);
    }
    else if (instr_is_branch(insn.id))
    {
        if (instr_is_conditional(insn.id))
        {
            // Conditional branch - has both fall-through and taken paths
            result.AddBranch(FalseBranch, addr + decoded_len);

            // Find branch target operand
            for (int i = 0; i < insn.operand_count; i++)
            {
                if (insn.operands[i].type == XTENSA_OP_BRANCH_TARGET)
                {
                    result.AddBranch(TrueBranch, insn.operands[i].target);
                    break;
                }
            }
        }
        else
        {
            // Unconditional branch
            if (insn.id == XTENSA_INS_JX)
            {
                // Indirect jump
                result.AddBranch(IndirectBranch);
            }
            else
            {
                // Direct jump (J)
                for (int i = 0; i < insn.operand_count; i++)
                {
                    if (insn.operands[i].type == XTENSA_OP_BRANCH_TARGET)
                    {
                        result.AddBranch(UnconditionalBranch, insn.operands[i].target);
                        break;
                    }
                }
            }
        }
    }
    else if (insn.id == XTENSA_INS_SYSCALL || insn.id == XTENSA_INS_SIMCALL)
    {
        // System calls
        result.AddBranch(SystemCall);
    }
    else if (insn.id == XTENSA_INS_ILL)
    {
        result.AddBranch(ExceptionBranch);
    }

    return true;
}

bool XtensaArchitecture::GetInstructionText(
    const uint8_t* data, uint64_t addr, size_t& len, vector<InstructionTextToken>& result)
{
    XtensaInstruction insn;

    // Decode instruction using xtensa_decoder
    int decoded_len = xtensa_decode(data, len, addr, &insn, m_options);
    if (decoded_len == 0)
        return false;

    len = decoded_len;

    // Add mnemonic token
    const char* mnemonic = xtensa_mnemonic(insn.id);
    result.emplace_back(InstructionToken, mnemonic);

    // Add spacing between mnemonic and operands
    if (insn.operand_count > 0)
    {
        size_t mnem_len = strlen(mnemonic);
        char spacing[16];
        memset(spacing, ' ', sizeof(spacing));
        spacing[mnem_len < 12 ? 12 - mnem_len : 1] = '\0';
        result.emplace_back(TextToken, spacing);
    }

    // Process each operand
    for (int i = 0; i < insn.operand_count; i++)
    {
        if (i > 0)
            result.emplace_back(OperandSeparatorToken, ", ");

        AddOperandTokens(result, insn.operands[i]);
    }

    return true;
}

bool XtensaArchitecture::GetInstructionLowLevelIL(
    const uint8_t* data, uint64_t addr, size_t& len, LowLevelILFunction& il)
{
    XtensaInstruction insn;
    int decoded_len = xtensa_decode(data, len, addr, &insn, m_options);
    if (decoded_len == 0)
        return false;

    len = decoded_len;

    return GetLowLevelILForInstruction(this, addr, il, insn);
}


//-----------------------------------------------------------------------------
// Architecture - Flags (Xtensa Boolean Option)
//-----------------------------------------------------------------------------

vector<uint32_t> XtensaArchitecture::GetAllFlags()
{
    // Boolean registers (b0-b15) used by FP compare and boolean branch instructions
    vector<uint32_t> flags;
    for (uint32_t i = 0; i < FLAG_COUNT; i++)
        flags.push_back(i);
    return flags;
}

string XtensaArchitecture::GetFlagName(uint32_t flag)
{
    if (flag < FLAG_COUNT)
        return XtensaFlagNames[flag];
    return "";
}

vector<uint32_t> XtensaArchitecture::GetAllFlagWriteTypes()
{
    return vector<uint32_t>();
}

string XtensaArchitecture::GetFlagWriteTypeName(uint32_t flags)
{
    return "";
}

BNFlagRole XtensaArchitecture::GetFlagRole(uint32_t flag, uint32_t semClass)
{
    return SpecialFlagRole;
}

vector<uint32_t> XtensaArchitecture::GetFlagsRequiredForFlagCondition(BNLowLevelILFlagCondition cond, uint32_t semClass)
{
    return vector<uint32_t>();
}

vector<uint32_t> XtensaArchitecture::GetAllSemanticFlagClasses()
{
    return vector<uint32_t>();
}

string XtensaArchitecture::GetSemanticFlagClassName(uint32_t semClass)
{
    return "";
}

vector<uint32_t> XtensaArchitecture::GetAllSemanticFlagGroups()
{
    return vector<uint32_t>();
}

string XtensaArchitecture::GetSemanticFlagGroupName(uint32_t semGroup)
{
    return "";
}

vector<uint32_t> XtensaArchitecture::GetFlagsWrittenByFlagWriteType(uint32_t writeType)
{
    return vector<uint32_t>();
}


//-----------------------------------------------------------------------------
// Architecture - Registers
//-----------------------------------------------------------------------------

vector<uint32_t> XtensaArchitecture::GetAllRegisters()
{
    vector<uint32_t> regs;
    for (uint32_t i = 0; i < REG_COUNT; i++)
    {
        regs.push_back(i);
    }
    return regs;
}

string XtensaArchitecture::GetRegisterName(uint32_t reg)
{
    if (reg < REG_COUNT)
    {
        return XtensaRegisterNames[reg];
    }
    return "";
}

BNRegisterInfo XtensaArchitecture::GetRegisterInfo(uint32_t reg)
{
    BNRegisterInfo info;
    info.fullWidthRegister = reg;
    info.offset = 0;
    info.size = 4;  // 32-bit registers
    info.extend = NoExtend;
    return info;
}

uint32_t XtensaArchitecture::GetStackPointerRegister()
{
    return REG_A1;  // a1 is the stack pointer
}

uint32_t XtensaArchitecture::GetLinkRegister()
{
    return REG_A0;  // a0 is the return address (link register)
}


//-----------------------------------------------------------------------------
// Architecture - Intrinsics
//-----------------------------------------------------------------------------

string XtensaArchitecture::GetIntrinsicName(uint32_t intrinsic)
{
    switch (intrinsic)
    {
    case ARCH_INTRINSIC_RSR:
        return "__rsr";
    case ARCH_INTRINSIC_WSR:
        return "__wsr";
    case ARCH_INTRINSIC_XSR:
        return "__xsr";
    case ARCH_INTRINSIC_ISYNC:
        return "__isync";
    case ARCH_INTRINSIC_RSYNC:
        return "__rsync";
    case ARCH_INTRINSIC_ESYNC:
        return "__esync";
    case ARCH_INTRINSIC_DSYNC:
        return "__dsync";
    case ARCH_INTRINSIC_MEMW:
        return "__memw";
    case ARCH_INTRINSIC_EXTW:
        return "__extw";
    case ARCH_INTRINSIC_S32C1I:
        return "__s32c1i";
    default:
        return "";
    }
}

vector<uint32_t> XtensaArchitecture::GetAllIntrinsics()
{
    return vector<uint32_t> {
        ARCH_INTRINSIC_RSR, ARCH_INTRINSIC_WSR, ARCH_INTRINSIC_XSR, ARCH_INTRINSIC_ISYNC,
        ARCH_INTRINSIC_RSYNC, ARCH_INTRINSIC_ESYNC, ARCH_INTRINSIC_DSYNC, ARCH_INTRINSIC_MEMW,
        ARCH_INTRINSIC_EXTW, ARCH_INTRINSIC_S32C1I};
}

vector<NameAndType> XtensaArchitecture::GetIntrinsicInputs(uint32_t intrinsic)
{
    auto int32Type = Type::IntegerType(4, false);

    switch (intrinsic)
    {
    case ARCH_INTRINSIC_RSR:
        // __rsr(sr_num) -> reads from special register
        return vector<NameAndType> {NameAndType("sr", Confidence(int32Type, 255))};
    case ARCH_INTRINSIC_WSR:
        // __wsr(sr_num, value) -> writes to special register
        return vector<NameAndType> {
            NameAndType("sr", Confidence(int32Type, 255)), NameAndType("value", Confidence(int32Type, 255))};
    case ARCH_INTRINSIC_XSR:
        // __xsr(sr_num, value) -> exchanges value with special register
        return vector<NameAndType> {
            NameAndType("sr", Confidence(int32Type, 255)), NameAndType("value", Confidence(int32Type, 255))};
    case ARCH_INTRINSIC_ISYNC:
    case ARCH_INTRINSIC_RSYNC:
    case ARCH_INTRINSIC_ESYNC:
    case ARCH_INTRINSIC_DSYNC:
    case ARCH_INTRINSIC_MEMW:
    case ARCH_INTRINSIC_EXTW:
        // Sync/barrier instructions have no inputs
        return vector<NameAndType>();
    case ARCH_INTRINSIC_S32C1I:
        // __s32c1i(addr, new_value) -> atomically: old = *addr; if (old == SCOMPARE1) *addr = new_value; return old;
        return vector<NameAndType> {NameAndType("addr", Confidence(Type::PointerType(4, int32Type), 255)),
            NameAndType("new_value", Confidence(int32Type, 255))};
    default:
        return vector<NameAndType>();
    }
}

vector<Confidence<Ref<Type>>> XtensaArchitecture::GetIntrinsicOutputs(uint32_t intrinsic)
{
    auto int32Type = Type::IntegerType(4, false);

    switch (intrinsic)
    {
    case ARCH_INTRINSIC_RSR:
        // Returns the value read from special register
        return vector<Confidence<Ref<Type>>> {Confidence(int32Type, 255)};
    case ARCH_INTRINSIC_XSR:
        // Returns the previous value of special register
        return vector<Confidence<Ref<Type>>> {Confidence(int32Type, 255)};
    case ARCH_INTRINSIC_WSR:
    case ARCH_INTRINSIC_ISYNC:
    case ARCH_INTRINSIC_RSYNC:
    case ARCH_INTRINSIC_ESYNC:
    case ARCH_INTRINSIC_DSYNC:
    case ARCH_INTRINSIC_MEMW:
    case ARCH_INTRINSIC_EXTW:
        // No return value
        return vector<Confidence<Ref<Type>>>();
    case ARCH_INTRINSIC_S32C1I:
        // Returns the old value from memory
        return vector<Confidence<Ref<Type>>> {Confidence(int32Type, 255)};
    default:
        return vector<Confidence<Ref<Type>>>();
    }
}


//-----------------------------------------------------------------------------
// Calling Convention Implementation
//-----------------------------------------------------------------------------

/* default */

XtensaDefaultCallingConvention::XtensaDefaultCallingConvention(Architecture* arch) : CallingConvention(arch, "default") {}

vector<uint32_t> XtensaDefaultCallingConvention::GetIntegerArgumentRegisters()
{
    // Xtensa uses a2-a7 for arguments (CALL0 ABI)
    return vector<uint32_t> {REG_A2, REG_A3, REG_A4, REG_A5, REG_A6, REG_A7};
}

vector<uint32_t> XtensaDefaultCallingConvention::GetCallerSavedRegisters()
{
    // a0 (return address), a2-a11 are caller-saved
    return vector<uint32_t> {REG_A0, REG_A2, REG_A3, REG_A4, REG_A5, REG_A6, REG_A7, REG_A8, REG_A9, REG_A10, REG_A11};
}

vector<uint32_t> XtensaDefaultCallingConvention::GetCalleeSavedRegisters()
{
    // a12-a15 are callee-saved
    return vector<uint32_t> {REG_A12, REG_A13, REG_A14, REG_A15};
}

uint32_t XtensaDefaultCallingConvention::GetIntegerReturnValueRegister()
{
    return REG_A2;  // a2 holds return value
}

uint32_t XtensaDefaultCallingConvention::GetHighIntegerReturnValueRegister()
{
    return REG_A3;  // a3 holds high part of 64-bit return
}

/* windowed */

XtensaWindowedCallingConvention::XtensaWindowedCallingConvention(Architecture* arch) :
    CallingConvention(arch, "windowed")
{}

vector<uint32_t> XtensaWindowedCallingConvention::GetIntegerArgumentRegisters()
{
    return vector<uint32_t> {REG_A2, REG_A3, REG_A4, REG_A5, REG_A6, REG_A7};
}

vector<uint32_t> XtensaWindowedCallingConvention::GetCallerSavedRegisters()
{
    return vector<uint32_t> {REG_A0, REG_A2, REG_A3, REG_A4, REG_A5, REG_A6, REG_A7};
}

vector<uint32_t> XtensaWindowedCallingConvention::GetCalleeSavedRegisters()
{
    return vector<uint32_t> {};
}

uint32_t XtensaWindowedCallingConvention::GetIntegerReturnValueRegister()
{
    return REG_A2;
}

uint32_t XtensaWindowedCallingConvention::GetHighIntegerReturnValueRegister()
{
    return REG_A3;
}


//-----------------------------------------------------------------------------
// Platform Implementation
//-----------------------------------------------------------------------------

class XtensaPlatform : public Platform
{
public:
    XtensaPlatform(Architecture* arch, const std::string& name) : Platform(arch, name)
    {
        Ref<CallingConvention> cc;
        cc = arch->GetCallingConventionByName("default");
        if (cc)
        {
            RegisterDefaultCallingConvention(cc);
            RegisterCdeclCallingConvention(cc);
        }
    }

    // quick hack of solving BN API issue 7697
    // https://github.com/Vector35/binaryninja-api/issues/7697
    virtual void AdjustTypeParserInput(Ref<TypeParser> parser, std::vector<std::string>& arguments,
        std::vector<std::pair<std::string, std::string>>& sourceFiles) override
    {
        if (parser->GetName() != "ClangTypeParser")
        {
            return;
        }

        for (auto& arg : arguments)
        {
            if (arg.find("--target=xtensa") == 0)
            {
                arg = "--target=i386-unknown-unknown-unknown";
            }
        }
    }
};


//-----------------------------------------------------------------------------
// Plugin Entry Point
//-----------------------------------------------------------------------------

extern "C"
{
    BN_DECLARE_CORE_ABI_VERSION

    BINARYNINJAPLUGIN bool CorePluginInit()
    {
        /*
         * Register Xtensa Core architecture
         */

        Architecture* xtensa_core_arch = new XtensaArchitecture("xtensa", LittleEndian, XTENSA_OPT_NONE);
        Architecture::Register(xtensa_core_arch);

        Ref<CallingConvention> xtensa_core_cc = new XtensaDefaultCallingConvention(xtensa_core_arch);
        xtensa_core_arch->RegisterCallingConvention(xtensa_core_cc);

        /*
         * Register ESP32 architecture & platform
         */

        Architecture* esp32_arch = new XtensaArchitecture(
            "esp32",
            LittleEndian, 
            (
                XTENSA_OPT_CODE_DENSITY | XTENSA_OPT_LOOP | XTENSA_OPT_BOOLEAN | XTENSA_OPT_WINDOWED |
                XTENSA_OPT_MUL32 | XTENSA_OPT_MUL16 | XTENSA_OPT_DIV32 | XTENSA_OPT_MAC16 |
                XTENSA_OPT_FP | XTENSA_OPT_COND_STORE | XTENSA_OPT_MISC | XTENSA_OPT_EXCEPTION2 |
                XTENSA_OPT_DEBUG
            )
        );
        Architecture::Register(esp32_arch);

        Ref<CallingConvention> esp32_cc_default = new XtensaDefaultCallingConvention(esp32_arch);
        esp32_arch->RegisterCallingConvention(esp32_cc_default);

        Ref<CallingConvention> esp32_cc_windowed = new XtensaWindowedCallingConvention(esp32_arch);
        esp32_arch->RegisterCallingConvention(esp32_cc_windowed);

        Ref<Platform> esp32_platform = new XtensaPlatform(esp32_arch, "esp32");
        Platform::Register("esp32", esp32_platform);

        esp32_platform->RegisterDefaultCallingConvention(esp32_cc_windowed);

        /*
         * Register ESP8266 architecture & platform
         */

        Architecture* esp8266_arch = new XtensaArchitecture(
            "esp8266",
            LittleEndian, 
            (
                XTENSA_OPT_CODE_DENSITY | XTENSA_OPT_MUL32 | XTENSA_OPT_MUL16 | XTENSA_OPT_MISC |
                XTENSA_OPT_EXCEPTION2 | XTENSA_OPT_DEBUG
            )
        );
        Architecture::Register(esp8266_arch);

        Ref<CallingConvention> esp8266_cc_default = new XtensaDefaultCallingConvention(esp8266_arch);
        esp8266_arch->RegisterCallingConvention(esp8266_cc_default);

        Ref<Platform> esp8266_platform = new XtensaPlatform(esp8266_arch, "esp8266");
        Platform::Register("esp8266", esp8266_platform);

        esp8266_platform->RegisterDefaultCallingConvention(esp8266_cc_default);

        return true;
    }
}
