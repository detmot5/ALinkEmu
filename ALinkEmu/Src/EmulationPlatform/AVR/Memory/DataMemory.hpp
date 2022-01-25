//
// Created by Norbert Bielak on 22.12.2021.
//
#pragma once
#include "Base/Base.hpp"
#include "EmulationPlatform/AVR/Utils.hpp"

namespace ALinkEmu::AVR {

struct AvrChip;
// Also known as RAM ;)
class DataMemory {
 public:
  explicit DataMemory(AvrChip& relatedChip);

  void Init();

  // Get/Set value from registers space
  // It can be used to IO and General Purpose registers
  // Will call ioWrite/ioRead Hook for given register if present
  // to notify IO module about access to its memory
  void SetRegisterValue(RamAddress address, uint8_t value);
  uint8_t GetRegisterValue(RamAddress address);

  // Gives access to whole memory - even for registers
  // The main difference is that it's not notifying IO modules about access to its memory
  void SetValueInMemory(RamAddress address, uint8_t value);
  uint8_t GetValueFromMemory(RamAddress address);

  void SetRegisterBit(RegisterBitLocation registerBit) {
    this->memory[registerBit.address] |= (1 << registerBit.offset);
  }

  void ClearRegisterBit(RegisterBitLocation registerBit) {
    this->memory[registerBit.address] &= ~(1 << registerBit.offset);
  }

  bool GetRegisterBit(RegisterBitLocation registerBit) {
    return this->memory[registerBit.address] & (1 << registerBit.offset);
  }

  void Clear();

 private:
  AvrChip& relatedChip;
  ByteBuffer memory;
  bool sregMirror[8] = {false};
};

}  // namespace ALinkEmu::AVR
