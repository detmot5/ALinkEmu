//
// Created by Norbert Bielak on 24.12.2021.
//

#include "ProgramMemory.hpp"

#include "EmulationPlatform/AVR/Chip/AvrChip.hpp"

namespace ALinkEmu::AVR {

ProgramMemory::ProgramMemory(AvrChip& relatedChip) : relatedChip(relatedChip) {}

void ProgramMemory::Init() {
  this->memory.Allocate(this->relatedChip.platformData.FLASHEND + 4);
  this->memory.Memset(0xFF);
}

uint32_t ProgramMemory::FetchInstruction(FlashAddress currentProgramCounter) {
  return this->memory[currentProgramCounter] | (this->memory[currentProgramCounter + 1] << 8);
}

bool ProgramMemory::LoadProgram(const ByteBuffer& buffer) {
  uint32_t flashEnd = this->relatedChip.platformData.FLASHEND;
  if (buffer.GetSize() < flashEnd) {
    this->memory.Memcpy(buffer);
    return true;
  } else {
    EMU_LOG_ERROR("Could not load program to memory, provided program is too big - {0}/{1}", buffer.GetSize(),
                  flashEnd);
    return false;
  }
}
bool ProgramMemory::LoadProgram(const uint8_t* buffer, size_t size) {
  uint32_t flashEnd = this->relatedChip.platformData.FLASHEND;
  if (size < flashEnd) {
    this->memory.Memcpy(buffer, size);
    return true;
  } else {
    EMU_LOG_ERROR("Could not load program to memory, provided program is too big - {0}/{1}", size, flashEnd);
    return false;
  }
}

void ProgramMemory::Clear() { this->memory.Memset(0xFF); }

}  // namespace ALinkEmu::AVR
