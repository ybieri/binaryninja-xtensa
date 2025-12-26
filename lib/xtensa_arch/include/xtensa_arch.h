#pragma once

#include "binaryninjaapi.h"
#include "binaryninjacore.h"


using namespace BinaryNinja;

using namespace std;


//-----------------------------------------------------------------------------
// Registers
//-----------------------------------------------------------------------------

enum XtensaRegister
{
    // General Purpose Registers
    REG_A0, REG_A1, REG_A2, REG_A3,
    REG_A4, REG_A5, REG_A6, REG_A7,
    REG_A8, REG_A9, REG_A10, REG_A11,
    REG_A12, REG_A13, REG_A14, REG_A15,

    // Program Counter
    REG_PC,

    // Shift Amount Register
    REG_SAR,

    // Loop Option Registers
    REG_LBEG, REG_LEND, REG_LCOUNT,

    // Floating-Point Option Registers
    REG_F0, REG_F1, REG_F2, REG_F3,
    REG_F4, REG_F5, REG_F6, REG_F7,
    REG_F8, REG_F9, REG_F10, REG_F11,
    REG_F12, REG_F13, REG_F14, REG_F15,

    REG_COUNT
};

static const char* XtensaRegisterNames[] = {
    "a0", "a1", "a2", "a3",
    "a4", "a5", "a6", "a7",
    "a8", "a9", "a10", "a11",
    "a12", "a13", "a14", "a15",

    "pc",

    "sar",

    "lbeg", "lend", "lcount",

    "f0", "f1", "f2", "f3",
    "f4", "f5", "f6", "f7",
    "f8", "f9", "f10", "f11",
    "f12", "f13", "f14", "f15"
};


//-----------------------------------------------------------------------------
// Flags (Xtensa Boolean Option)
//-----------------------------------------------------------------------------

enum XtensaFlag
{
    FLAG_B0 = 0,
    FLAG_B1,
    FLAG_B2,
    FLAG_B3,
    FLAG_B4,
    FLAG_B5,
    FLAG_B6,
    FLAG_B7,
    FLAG_B8,
    FLAG_B9,
    FLAG_B10,
    FLAG_B11,
    FLAG_B12,
    FLAG_B13,
    FLAG_B14,
    FLAG_B15,

    FLAG_COUNT
};

static const char* XtensaFlagNames[] = {
    "b0", "b1", "b2", "b3",
    "b4", "b5", "b6", "b7",
    "b8", "b9", "b10", "b11",
    "b12", "b13", "b14", "b15"
};


//-----------------------------------------------------------------------------
// Intrinsics
//-----------------------------------------------------------------------------

enum XtensaIntrinsic
{
    ARCH_INTRINSIC_RSR = 0,
    ARCH_INTRINSIC_WSR,
    ARCH_INTRINSIC_XSR,
    ARCH_INTRINSIC_ISYNC,
    ARCH_INTRINSIC_RSYNC,
    ARCH_INTRINSIC_ESYNC,
    ARCH_INTRINSIC_DSYNC,
    ARCH_INTRINSIC_MEMW,
    ARCH_INTRINSIC_EXTW,
    ARCH_INTRINSIC_S32C1I,

    ARCH_INTRINSIC_COUNT
};


//-----------------------------------------------------------------------------
// Architecture
//-----------------------------------------------------------------------------

class XtensaArchitecture : public Architecture
{
private:
    BNEndianness m_endian;
    size_t m_addressSize;
    uint32_t m_options;

public:
    XtensaArchitecture(const char* name, BNEndianness endian, uint32_t options);

    uint32_t GetOptions() const { return m_options; }

    // Core Informations
    virtual BNEndianness GetEndianness() const override;
    virtual size_t GetAddressSize() const override;
    virtual size_t GetDefaultIntegerSize() const override;
    virtual size_t GetInstructionAlignment() const override;
    virtual size_t GetMaxInstructionLength() const override;

    // Core Disassembly & Lifting Methods
    virtual bool GetInstructionInfo(
        const uint8_t* data, uint64_t addr, size_t maxLen, InstructionInfo& result) override;

    virtual bool GetInstructionText(
        const uint8_t* data, uint64_t addr, size_t& len, vector<InstructionTextToken>& result) override;

    virtual bool GetInstructionLowLevelIL(
        const uint8_t* data, uint64_t addr, size_t& len, LowLevelILFunction& il) override;

    // Registers
    virtual vector<uint32_t> GetAllRegisters() override;
    virtual string GetRegisterName(uint32_t reg) override;
    virtual BNRegisterInfo GetRegisterInfo(uint32_t reg) override;
    virtual uint32_t GetStackPointerRegister() override;
    virtual uint32_t GetLinkRegister() override;

    // Flags
    virtual vector<uint32_t> GetAllFlags() override;
    virtual string GetFlagName(uint32_t flag) override;
    virtual vector<uint32_t> GetAllFlagWriteTypes() override;
    virtual string GetFlagWriteTypeName(uint32_t flags) override;
    virtual BNFlagRole GetFlagRole(uint32_t flag, uint32_t semClass) override;
    virtual vector<uint32_t> GetFlagsRequiredForFlagCondition(
        BNLowLevelILFlagCondition cond, uint32_t semClass) override;

    virtual vector<uint32_t> GetAllSemanticFlagClasses() override;
    virtual string GetSemanticFlagClassName(uint32_t semClass) override;
    virtual vector<uint32_t> GetAllSemanticFlagGroups() override;
    virtual string GetSemanticFlagGroupName(uint32_t semGroup) override;
    virtual vector<uint32_t> GetFlagsWrittenByFlagWriteType(uint32_t writeType) override;

    // Intrinsics
    virtual string GetIntrinsicName(uint32_t intrinsic) override;
    virtual vector<uint32_t> GetAllIntrinsics() override;
    virtual vector<NameAndType> GetIntrinsicInputs(uint32_t intrinsic) override;
    virtual vector<Confidence<Ref<Type>>> GetIntrinsicOutputs(uint32_t intrinsic) override;
};


//-----------------------------------------------------------------------------
// Calling Convention
//-----------------------------------------------------------------------------

class XtensaDefaultCallingConvention : public CallingConvention
{
public:
    XtensaDefaultCallingConvention(Architecture* arch);

    virtual vector<uint32_t> GetIntegerArgumentRegisters() override;
    virtual vector<uint32_t> GetCallerSavedRegisters() override;
    virtual vector<uint32_t> GetCalleeSavedRegisters() override;
    virtual uint32_t GetIntegerReturnValueRegister() override;
    virtual uint32_t GetHighIntegerReturnValueRegister() override;
};

class XtensaWindowedCallingConvention : public CallingConvention
{
public:
    XtensaWindowedCallingConvention(Architecture* arch);

    virtual vector<uint32_t> GetIntegerArgumentRegisters() override;
    virtual vector<uint32_t> GetCallerSavedRegisters() override;
    virtual vector<uint32_t> GetCalleeSavedRegisters() override;
    virtual uint32_t GetIntegerReturnValueRegister() override;
    virtual uint32_t GetHighIntegerReturnValueRegister() override;
};
