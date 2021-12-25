//
// Created by Norbert Bielak on 24.12.2021.
//
#pragma once
#include "Base/Base.hpp"

#include "EmulationPlatform/AVR/Utils.hpp"

namespace ALinkEmu::AVR {

struct AvrChip;

class ProgramMemory {
 public:
  explicit ProgramMemory(AvrChip& relatedChip);

  void Init();
  uint32_t FetchInstruction(FlashAddress currentProgramCounter);
  bool LoadProgram(const ByteBuffer& buffer);
  bool LoadProgram(const uint8_t* buffer, size_t size);
  void Clear();


 private:
  AvrChip& relatedChip;
  ByteBuffer memory;
};

}  // namespace ALinkEmu::AVR
