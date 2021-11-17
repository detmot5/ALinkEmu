//
// Created by norbert on 17.11.2021.
//

#include "Core.hpp"

#include <cstring>

namespace ALinkEmu::AVR {
void Core::Init() {
  this->state = CpuState::BEFORE_INIT;
  this->flash = std::make_unique<uint16_t[]>(this->platformDependentData.FLASHEND + 4);
  std::memset(this->flash.get(), 0xFF, this->platformDependentData.FLASHEND + 1);
  this->codeEnd = this->platformDependentData.FLASHEND;
  this->ram = std::make_unique<uint8_t[]>(this->platformDependentData.RAMEND + 1);
  std::memset(this->ram.get(), 0x00, this->platformDependentData.RAMEND);
  this->frequency = 10000000;
}

void Core::Reset() {}
void Core::Shutdown() {}
void Core::ExecuteSingleInstruction() {}
uint32_t Core::FetchInstruction(FlashAddress currentPC) { return 0; }

}

