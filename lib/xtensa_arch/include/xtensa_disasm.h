#pragma once

#include "binaryninjaapi.h"
#include "xtensa_decoder.h"

// Helper function to add operand tokens for disassembly
void AddOperandTokens(std::vector<BinaryNinja::InstructionTextToken>& result, const XtensaOperand& op);
