#pragma once

#include <cstdint>
#include <cstddef>


//-----------------------------------------------------------------------------
// Architecture Options
//-----------------------------------------------------------------------------

enum XtensaOption : uint32_t
{
    XTENSA_OPT_NONE         = 0,
    XTENSA_OPT_CODE_DENSITY = (1 << 0),
    XTENSA_OPT_LOOP         = (1 << 1),
    XTENSA_OPT_MUL16        = (1 << 2),
    XTENSA_OPT_MUL32        = (1 << 3),
    XTENSA_OPT_DIV32        = (1 << 4),
    XTENSA_OPT_MAC16        = (1 << 5),
    XTENSA_OPT_MISC         = (1 << 6),
    XTENSA_OPT_DEPBITS      = (1 << 7),
    XTENSA_OPT_BOOLEAN      = (1 << 8),
    XTENSA_OPT_FP           = (1 << 9),
    XTENSA_OPT_SYNC         = (1 << 10),
    XTENSA_OPT_COND_STORE   = (1 << 11),
    XTENSA_OPT_EXCEPTION2   = (1 << 12),
    XTENSA_OPT_INTERRUPT    = (1 << 13),
    XTENSA_OPT_WINDOWED     = (1 << 14),
    XTENSA_OPT_DEBUG        = (1 << 15),
};


constexpr uint32_t XTENSA_PRESET_ESP8266 = (
    XTENSA_OPT_CODE_DENSITY |
    XTENSA_OPT_MUL32        |
    XTENSA_OPT_MUL16        |
    XTENSA_OPT_MISC         |
    XTENSA_OPT_EXCEPTION2   |
    XTENSA_OPT_DEBUG
);


constexpr uint32_t XTENSA_PRESET_ESP32 = (
    XTENSA_OPT_CODE_DENSITY |
    XTENSA_OPT_LOOP         |
    XTENSA_OPT_BOOLEAN      |
    XTENSA_OPT_WINDOWED     |
    XTENSA_OPT_MUL32        |
    XTENSA_OPT_MUL16        |
    XTENSA_OPT_DIV32        |
    XTENSA_OPT_MAC16        |
    XTENSA_OPT_FP           |
    XTENSA_OPT_COND_STORE   |
    XTENSA_OPT_MISC         |
    XTENSA_OPT_EXCEPTION2   |
    XTENSA_OPT_DEBUG
);


//-----------------------------------------------------------------------------
// Instructions
//-----------------------------------------------------------------------------


enum XtensaFormat
{
    XTENSA_FMT_INVALID = 0,
    XTENSA_FMT_RRR,
    XTENSA_FMT_RRI4,
    XTENSA_FMT_RRI8,
    XTENSA_FMT_RI16,
    XTENSA_FMT_RSR,
    XTENSA_FMT_CALL,
    XTENSA_FMT_CALLX,
    XTENSA_FMT_BRI8,
    XTENSA_FMT_BRI12,
    XTENSA_FMT_RRRN,
    XTENSA_FMT_RI6,
    XTENSA_FMT_RI7,
};

enum XtensaInsnId
{
    XTENSA_INS_INVALID = 0,

    // Arithmetic & Logical (RRR format mostly)
    XTENSA_INS_ADD,
    XTENSA_INS_ADDI,
    XTENSA_INS_ADDMI,
    XTENSA_INS_ADDX2,
    XTENSA_INS_ADDX4,
    XTENSA_INS_ADDX8,
    XTENSA_INS_SUB,
    XTENSA_INS_SUBX2,
    XTENSA_INS_SUBX4,
    XTENSA_INS_SUBX8,
    XTENSA_INS_AND,
    XTENSA_INS_OR,
    XTENSA_INS_XOR,
    XTENSA_INS_NEG,
    XTENSA_INS_ABS,

    // RFW
    XTENSA_INS_RFWO,
    XTENSA_INS_RFWU,

    // Move operations
    XTENSA_INS_MOV,
    XTENSA_INS_MOVI,
    XTENSA_INS_MOVEQZ,
    XTENSA_INS_MOVNEZ,
    XTENSA_INS_MOVLTZ,
    XTENSA_INS_MOVGEZ,

    // Shift operations
    XTENSA_INS_SLL,
    XTENSA_INS_SLLI,
    XTENSA_INS_SRL,
    XTENSA_INS_SRLI,
    XTENSA_INS_SRA,
    XTENSA_INS_SRAI,
    XTENSA_INS_SRC,
    XTENSA_INS_SSL,
    XTENSA_INS_SSR,
    XTENSA_INS_SSA8L,
    XTENSA_INS_SSA8B,
    XTENSA_INS_SSAI,

    // Multiply operations
    XTENSA_INS_MULL,    // Multiply low 32-bit
    XTENSA_INS_MULUH,   // Multiply unsigned high 32-bit
    XTENSA_INS_MULSH,   // Multiply signed high 32-bit
    XTENSA_INS_MUL16U,  // Multiply 16-bit unsigned
    XTENSA_INS_MUL16S,  // Multiply 16-bit signed

    // Divide and remainder operations
    XTENSA_INS_QUOU,  // Quotient unsigned
    XTENSA_INS_QUOS,  // Quotient signed
    XTENSA_INS_REMU,  // Remainder unsigned
    XTENSA_INS_REMS,  // Remainder signed

    // Miscellaneous operations
    XTENSA_INS_NSA,   // Normalization shift amount
    XTENSA_INS_NSAU,  // Normalization shift amount unsigned
    XTENSA_INS_MIN,   // Minimum signed
    XTENSA_INS_MAX,   // Maximum signed
    XTENSA_INS_MINU,  // Minimum unsigned
    XTENSA_INS_MAXU,  // Maximum unsigned

    // Load operations
    XTENSA_INS_L8UI,   // Load 8-bit unsigned
    XTENSA_INS_L16UI,  // Load 16-bit unsigned
    XTENSA_INS_L16SI,  // Load 16-bit signed
    XTENSA_INS_L32I,   // Load 32-bit
    XTENSA_INS_L32R,   // Load 32-bit PC-relative

    // Store operations
    XTENSA_INS_S8I,   // Store 8-bit
    XTENSA_INS_S16I,  // Store 16-bit
    XTENSA_INS_S32I,  // Store 32-bit

    // Atomic operations
    XTENSA_INS_L32AI,   // Load 32-bit atomic
    XTENSA_INS_S32C1I,  // Store 32-bit conditional
    XTENSA_INS_S32RI,   // Store 32-bit release

    // MAC16 operations
    XTENSA_INS_MAC16,  // Generic MAC16 instruction

    // Floating-Point Arithmetic (Single Precision) - FP0 table (op1=0b1010)
    XTENSA_INS_ADD_S,    // Add single
    XTENSA_INS_SUB_S,    // Subtract single
    XTENSA_INS_MUL_S,    // Multiply single
    XTENSA_INS_MADD_S,   // Multiply and add single
    XTENSA_INS_MSUB_S,   // Multiply and subtract single
    XTENSA_INS_MADDN_S,  // Multiply and add (normalized)
    XTENSA_INS_DIVN_S,   // Divide step (normalized)

    // Floating-Point Conversion (Single Precision)
    XTENSA_INS_ROUND_S,   // Round single to fixed
    XTENSA_INS_TRUNC_S,   // Truncate single to fixed
    XTENSA_INS_FLOOR_S,   // Floor single to fixed
    XTENSA_INS_CEIL_S,    // Ceiling single to fixed
    XTENSA_INS_FLOAT_S,   // Convert fixed to single
    XTENSA_INS_UFLOAT_S,  // Convert unsigned fixed to single
    XTENSA_INS_UTRUNC_S,  // Truncate single to unsigned fixed

    // Floating-Point Unary Operations - FP1OP table (op1=0b1010, op2=0b1111)
    XTENSA_INS_MOV_S,      // Move single
    XTENSA_INS_ABS_S,      // Absolute value single
    XTENSA_INS_NEG_S,      // Negate single
    XTENSA_INS_RFR,        // Move FR to AR
    XTENSA_INS_WFR,        // Move AR to FR
    XTENSA_INS_CONST_S,    // Load FP constant
    XTENSA_INS_DIV0_S,     // Divide step 0
    XTENSA_INS_RECIP0_S,   // Reciprocal approximation
    XTENSA_INS_SQRT0_S,    // Square root step 0
    XTENSA_INS_RSQRT0_S,   // Reciprocal square root approximation
    XTENSA_INS_NEXP01_S,   // Negate exponent for div/sqrt
    XTENSA_INS_MKSADJ_S,   // Make single adjustment
    XTENSA_INS_MKDADJ_S,   // Make double adjustment (for single precision result)
    XTENSA_INS_ADDEXP_S,   // Add exponents
    XTENSA_INS_ADDEXPM_S,  // Add exponent from mantissa

    // Floating-Point Compare - FP1 table (op1=0b1011)
    XTENSA_INS_UN_S,   // Unordered compare
    XTENSA_INS_OEQ_S,  // Ordered equal
    XTENSA_INS_UEQ_S,  // Unordered or equal
    XTENSA_INS_OLT_S,  // Ordered less than
    XTENSA_INS_ULT_S,  // Unordered or less than
    XTENSA_INS_OLE_S,  // Ordered less or equal
    XTENSA_INS_ULE_S,  // Unordered or less or equal

    // Floating-Point Conditional Move - FP1 table (op1=0b1011)
    XTENSA_INS_MOVEQZ_S,  // Move if AR equal zero
    XTENSA_INS_MOVNEZ_S,  // Move if AR not equal zero
    XTENSA_INS_MOVLTZ_S,  // Move if AR less than zero
    XTENSA_INS_MOVGEZ_S,  // Move if AR greater or equal zero
    XTENSA_INS_MOVF_S,    // Move if boolean false
    XTENSA_INS_MOVT_S,    // Move if boolean true

    // Floating-Point Load/Store Immediate (op0=3)
    XTENSA_INS_LSI,   // Load single immediate
    XTENSA_INS_SSI,   // Store single immediate
    XTENSA_INS_LSIU,  // Load single immediate with update
    XTENSA_INS_SSIU,  // Store single immediate with update

    // Floating-Point Load/Store Indexed - LSCX table (op1=0b1000)
    XTENSA_INS_LSX,   // Load single indexed
    XTENSA_INS_LSXU,  // Load single indexed with update
    XTENSA_INS_SSX,   // Store single indexed
    XTENSA_INS_SSXU,  // Store single indexed with update

    // Unconditional branches
    XTENSA_INS_J,   // Jump
    XTENSA_INS_JX,  // Jump register

    // Zero-Overhead Loops
    XTENSA_INS_LOOP,     // Loop
    XTENSA_INS_LOOPNEZ,  // Loop if not equal to zero
    XTENSA_INS_LOOPGTZ,  // Loop if greater than zero

    // Conditional branches (compare and branch)
    XTENSA_INS_BEQ,   // Branch if equal
    XTENSA_INS_BNE,   // Branch if not equal
    XTENSA_INS_BLT,   // Branch if less than
    XTENSA_INS_BLTU,  // Branch if less than unsigned
    XTENSA_INS_BGE,   // Branch if greater or equal
    XTENSA_INS_BGEU,  // Branch if greater or equal unsigned
    XTENSA_INS_BEQZ,  // Branch if equal to zero
    XTENSA_INS_BNEZ,  // Branch if not equal to zero
    XTENSA_INS_BLTZ,  // Branch if less than zero
    XTENSA_INS_BGEZ,  // Branch if greater or equal to zero

    // Bit test branches
    XTENSA_INS_BALL,   // Branch if all bits set
    XTENSA_INS_BANY,   // Branch if any bit set
    XTENSA_INS_BBC,    // Branch if bit clear
    XTENSA_INS_BBS,    // Branch if bit set
    XTENSA_INS_BBCI,   // Branch if bit clear immediate
    XTENSA_INS_BBSI,   // Branch if bit set immediate
    XTENSA_INS_BNONE,  // Branch if none (AND result is 0)
    XTENSA_INS_BNALL,  // Branch if not all bits set

    // Immediate compare branches
    XTENSA_INS_BEQI,   // Branch if equal immediate
    XTENSA_INS_BNEI,   // Branch if not equal immediate
    XTENSA_INS_BLTI,   // Branch if less than immediate
    XTENSA_INS_BLTUI,  // Branch if less than unsigned immediate
    XTENSA_INS_BGEI,   // Branch if greater or equal immediate
    XTENSA_INS_BGEUI,  // Branch if greater or equal unsigned immediate

    // Boolean branches (Boolean Option)
    XTENSA_INS_BF,  // Branch if False
    XTENSA_INS_BT,  // Branch if True

    // Call operations (windowed ABI)
    XTENSA_INS_CALL0,    // Call with register window increment 0
    XTENSA_INS_CALL4,    // Call with register window increment 4
    XTENSA_INS_CALL8,    // Call with register window increment 8
    XTENSA_INS_CALL12,   // Call with register window increment 12
    XTENSA_INS_CALLX0,   // Call register with window increment 0
    XTENSA_INS_CALLX4,   // Call register with window increment 4
    XTENSA_INS_CALLX8,   // Call register with window increment 8
    XTENSA_INS_CALLX12,  // Call register with window increment 12

    // Return operations
    XTENSA_INS_RET,   // Return (alias for JX a0)
    XTENSA_INS_RETW,  // Return with window decrement

    // Windowed register operations
    XTENSA_INS_ENTRY,  // Function entry (adjust stack, rotate window)
    XTENSA_INS_MOVSP,  // Move stack pointer
    XTENSA_INS_ROTW,   // Rotate window

    // Special register access
    XTENSA_INS_RSR,  // Read special register
    XTENSA_INS_WSR,  // Write special register
    XTENSA_INS_XSR,  // Exchange special register

    // System/Control
    XTENSA_INS_NOP,
    XTENSA_INS_RFE,   // Return from exception
    XTENSA_INS_RFDE,  // Return from double exception
    XTENSA_INS_EXCW,  // Exception wait
    XTENSA_INS_SYSCALL,
    XTENSA_INS_SIMCALL,
    XTENSA_INS_BREAK,
    XTENSA_INS_MEMW,  // Memory barrier
    XTENSA_INS_EXTW,
    XTENSA_INS_ISYNC,
    XTENSA_INS_RSYNC,
    XTENSA_INS_ESYNC,
    XTENSA_INS_DSYNC,

    // Code Density - 16-bit narrow instructions
    XTENSA_INS_ADD_N,    // Narrow add
    XTENSA_INS_ADDI_N,   // Narrow add immediate
    XTENSA_INS_MOV_N,    // Narrow move
    XTENSA_INS_MOVI_N,   // Narrow move immediate
    XTENSA_INS_L32I_N,   // Narrow 32-bit load
    XTENSA_INS_S32I_N,   // Narrow 32-bit store
    XTENSA_INS_BEQZ_N,   // Narrow branch if zero
    XTENSA_INS_BNEZ_N,   // Narrow branch if not zero
    XTENSA_INS_RET_N,    // Narrow return
    XTENSA_INS_RETW_N,   // Narrow return with window
    XTENSA_INS_BREAK_N,  // Narrow breakpoint
    XTENSA_INS_NOP_N,    // Narrow NOP
    XTENSA_INS_ILL_N,    // Narrow illegal instruction

    // Additional common instructions
    XTENSA_INS_EXTUI,    // Extract unsigned immediate
    XTENSA_INS_SEXT,     // Sign extend
    XTENSA_INS_CLAMPS,   // Signed clamp
    XTENSA_INS_DEPBITS,  // Deposit bits

    XTENSA_INS_ILL,
    XTENSA_INS_MAX_VALUE
};



enum XtensaOperandType
{
    XTENSA_OP_INVALID = 0,
    XTENSA_OP_REG,
    XTENSA_OP_FPREG,
    XTENSA_OP_BREG,
    XTENSA_OP_IMM,
    XTENSA_OP_MEM,
    XTENSA_OP_BRANCH_TARGET,
    XTENSA_OP_SREG,
};


struct XtensaOperand
{
    XtensaOperandType type;

    union
    {
        uint32_t reg;
        int32_t imm;
        struct
        {
            uint32_t base;
            int32_t offset;
        } mem;
        uint64_t target;
        uint32_t sreg;
        uint64_t _r;
    };

    XtensaOperand() : type(XTENSA_OP_INVALID) { _r = 0; }
};


struct XtensaInstruction
{
    XtensaInsnId id;
    XtensaFormat format;
    uint8_t length;
    uint32_t raw;
    const char* mnemonic;
    XtensaOperand operands[4];
    uint8_t operand_count;

    XtensaInstruction()
    {
        id = XTENSA_INS_INVALID;
        format = XTENSA_FMT_INVALID;
        length = 0;
        raw = 0;
        mnemonic = "invalid";
        operand_count = 0;
    }
};


int xtensa_decode(const uint8_t* data, size_t maxLen, uint64_t addr, XtensaInstruction* insn, uint32_t options);


const char* xtensa_mnemonic(XtensaInsnId id);
