//
// Created by Norbert Bielak on 18.11.2021.
//
#pragma once
#include <cstdint>
#include <utility>

#include "Base/Base.hpp"
#include "EmulationPlatform/AVR/InstructionExecutor/AddressingModeDecoder.hpp"

namespace ALinkEmu::AVR {


class Core;

class InstructionExecutor {
 public:
  void AttachCore(Core* coreRef);

  // Compare With Carry
  void CPC(uint32_t opcode);
  // Add Without Carry
  void ADD(uint32_t opcode);
  // Subtract With Carry
  void SBC(uint32_t opcode);



 private:
  Core* coreRef;
};

}  // namespace ALinkEmu::AVR
