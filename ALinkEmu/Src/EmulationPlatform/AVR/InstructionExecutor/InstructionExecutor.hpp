//
// Created by Norbert Bielak on 18.11.2021.
//
#pragma once
#include <cstdint>
#include <functional>
#include <utility>

#include "Base/Base.hpp"
#include "EmulationPlatform/AVR/InstructionExecutor/AddressingModeDecoder.hpp"

namespace ALinkEmu::AVR {

class Core;

class InstructionExecutor {
public:
  explicit InstructionExecutor(Core *coreRef) : coreRef(coreRef) {}

  // Compare With Carry
  void CPC(uint32_t opcode);
  // Add Without Carry
  void ADD(uint32_t opcode);
  // Subtract With Carry
  void SBC(uint32_t opcode);
  // Copy Register Word
  void MOVW(uint32_t opcode);
  // Multiply Signed
  void MULS(uint32_t opcode);
  // Multiply Signed With Unsigned
  void MULSU(uint32_t opcode);
  // Fractional Multiply Unsigned
  void FMUL(uint32_t opcode); 
  // Fractional Multiply Signed
  void FMULS(uint32_t opcode);
  // Fractional Multiply Signed With Unsigned
  void FMULSU(uint32_t opcode);

  // OUT - Write to IO space
  void OUT(uint32_t opcode);
  // IN - Read from IO space
  void IN(uint32_t opcode);

private:
  Core *coreRef;
};

} // namespace ALinkEmu::AVR
