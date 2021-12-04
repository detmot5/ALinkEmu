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
  explicit InstructionExecutor(Core* coreRef) : coreRef(coreRef) {}

  // Compare With Carry
  void CPC(uint32_t opcode);
  // Add Without Carry
  void ADD(uint32_t opcode);
  // Subtract With Carry
  void SBC(uint32_t opcode);
  // Copy Register Word
  void MOVW(uint32_t opcode);
  // Multiply signed
  void MULS(uint32_t opcode);
  // Multiply Unsigned
  void MUL(uint32_t opcode);
  //
  void MULSU(uint32_t opcode);
  //
  void FMUL(uint32_t opcode); 
  //
  void FMULS(uint32_t opcode);
  //
  void FMULSU(uint32_t opcode);

 private:
  Core* coreRef;
};

}  // namespace ALinkEmu::AVR
