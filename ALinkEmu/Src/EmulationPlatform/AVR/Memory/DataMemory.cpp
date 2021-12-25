//
// Created by Norbert Bielak on 22.12.2021.
//

#include "DataMemory.hpp"

#include "EmulationPlatform/AVR/Chip/AvrChip.hpp"

namespace ALinkEmu::AVR {

DataMemory::DataMemory(AvrChip& relatedChip) : relatedChip(relatedChip) {}

void DataMemory::Init() {
  this->memory.Allocate(relatedChip.platformData.RAMEND);
  this->memory.Memset(0x00);
}

void DataMemory::SetRegisterValue(RamAddress address, uint8_t value) {
  this->SetValueInMemory(address, value);
  // If writing to IO space
  if (address > 31) {
    // notify IO module about writing to its memory
    this->relatedChip.ioController.CallIoWriteHook(address);
  }
}

uint8_t DataMemory::GetRegisterValue(RamAddress address) {
  uint8_t value = this->GetValueFromMemory(address);
  // If reading from IO Space
  if (address > 31) {
    // notify IO module about access to its memory
    this->relatedChip.ioController.CallIoReadHook(address);
  }
  return value;
}

void DataMemory::SetValueInMemory(RamAddress address, uint8_t value) {
  try {
    this->memory[address] = value;
  } catch (ArrayBufferOutOfRangeException& ex) {
    EMU_LOG_ERROR("Data (RAM) memory {0} Tried to write to address bigger than RAMEND", ex.what());
    this->relatedChip.Abort();
  }
}

uint8_t DataMemory::GetValueFromMemory(RamAddress address) {
  try {
    return this->memory[address];
  } catch (ArrayBufferOutOfRangeException& ex) {
    EMU_LOG_ERROR("Data (RAM) memory {0} Tried to read from address bigger than RAMEND", ex.what());
    this->relatedChip.Abort();
  }
  return 0;
}

void DataMemory::Clear() { this->memory.Memset(0x00); }

}  // namespace ALinkEmu::AVR
