#include "xtensa_il.h"
#include "xtensa_arch.h"
#include <inttypes.h>


bool GetLowLevelILForInstruction(
    Architecture* arch,
    uint64_t addr,
    LowLevelILFunction& il,
    XtensaInstruction& insn
) {
    LLIL_LIFT(core_mem_load);
    LLIL_LIFT(core_mem_store);
    LLIL_LIFT(core_mem_order);
    LLIL_LIFT(core_jump);
    LLIL_LIFT(core_call);
    LLIL_LIFT(core_branch);
    LLIL_LIFT(core_move);
    LLIL_LIFT(core_arithmetic);
    LLIL_LIFT(core_logical);
    LLIL_LIFT(core_shift);
    LLIL_LIFT(core_pc);

    LLIL_LIFT(opt_boolean);
    LLIL_LIFT(opt_cond_store);
    LLIL_LIFT(opt_depbits);
    LLIL_LIFT(opt_exception2);
    LLIL_LIFT(opt_fp);
    LLIL_LIFT(opt_interrupt);
    LLIL_LIFT(opt_loop);
    LLIL_LIFT(opt_misc);
    LLIL_LIFT(opt_narrow);
    LLIL_LIFT(opt_sync);
    LLIL_LIFT(opt_windowed);

    switch (insn.id)
    {
        case XTENSA_INS_MAC16:
        {
            if (insn.operand_count != 3)
            {
                LogWarn("MAC16 instruction at 0x%" PRIx64 " has unexpected operand count: %d", addr, insn.operand_count);
                il.AddInstruction(il.Undefined());
                break;
            }
            il.AddInstruction(il.Unimplemented());
            return true;
        }

        default: break;
    }

    return false;
}
