#pragma once

#include "binaryninjaapi.h"
#include "lowlevelilinstruction.h"
#include "xtensa_decoder.h"

using namespace BinaryNinja;

#define LLIL_LIFTER(t) bool GetLowLevelILForInstruction_ ## t (Architecture* arch, uint64_t addr, LowLevelILFunction& il, XtensaInstruction& insn)

#define _LLIL_LIFT(t, a, b, c, d) GetLowLevelILForInstruction_ ## t (a, b, c, d)

#define LLIL_LIFT(t) if (_LLIL_LIFT(t, arch, addr, il, insn) == true) return true

LLIL_LIFTER(core_mem_load);
LLIL_LIFTER(core_mem_store);
LLIL_LIFTER(core_mem_order);
LLIL_LIFTER(core_jump);
LLIL_LIFTER(core_call);
LLIL_LIFTER(core_branch);
LLIL_LIFTER(core_move);
LLIL_LIFTER(core_arithmetic);
LLIL_LIFTER(core_logical);
LLIL_LIFTER(core_shift);
LLIL_LIFTER(core_pc);
LLIL_LIFTER(core);

LLIL_LIFTER(opt_boolean);
LLIL_LIFTER(opt_cond_store);
LLIL_LIFTER(opt_depbits);
LLIL_LIFTER(opt_exception2);
LLIL_LIFTER(opt_fp);
LLIL_LIFTER(opt_interrupt);
LLIL_LIFTER(opt_loop);
LLIL_LIFTER(opt_misc);
LLIL_LIFTER(opt_narrow);
LLIL_LIFTER(opt_sync);
LLIL_LIFTER(opt_windowed);
LLIL_LIFTER(opt_muldiv);

bool GetLowLevelILForInstruction(
    Architecture* arch,
    uint64_t addr,
    LowLevelILFunction& il,
    XtensaInstruction& insn
);
