//
// Created by norbert on 17.11.2021.
//

#include "Core.hpp"

#include <cstring>

#include "Base/Base.hpp"
#include "EmulationPlatform/AVR/InstructionExecutor/InstructionExecutor.hpp"

namespace ALinkEmu::AVR {

void Core::Init() {
  this->state = CpuState::BEFORE_INIT;
  this->flash = std::make_shared<uint16_t[]>(this->platformDependentData.FLASHEND + 4);
  std::memset(this->flash.get(), 0xFF, this->platformDependentData.FLASHEND + 1);
  this->codeEnd = this->platformDependentData.FLASHEND;
  this->ram = std::make_shared<uint8_t[]>(this->platformDependentData.RAMEND + 1);
  std::memset(this->ram.get(), 0x00, this->platformDependentData.RAMEND);
  this->frequency = 10000000;
  this->instructionExecutor.AttachCore(this);
  this->PC = 0;

  this->ram[0] = 10;
  this->ram[1] = 20;
}

void Core::Reset() {}
void Core::Shutdown() {}
void Core::ExecuteSingleInstruction() {
  uint32_t opcode = this->FetchInstruction(this->PC);
  opcode <<= 8;
  uint32_t newPC = this->PC + 2;

  EMU_LOG_WARN("{0}", opcode & 0xFC00);

  switch (opcode & 0xF000) {
    case 0x0000: {
      switch (opcode) {
        case 0x0000: {
          EMU_LOG_WARN("NOP");
        } break;
        default: {
          switch (opcode & 0xFC00) {
            case 0x0400: {
              this->instructionExecutor.CPC(opcode);
            } break;
            case 0x0C00: {
              this->instructionExecutor.ADD(opcode);
            } break;
          }
        }
      }
    }
  }
}

uint32_t Core::FetchInstruction(FlashAddress currentPC) { return this->flash[PC]; }

void Core::LoadFirmware(uint8_t* data, size_t size) { std::memcpy(this->flash.get(), data, size); }

}  // namespace ALinkEmu::AVR
