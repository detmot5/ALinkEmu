//
// Created by norbert on 17.11.2021.
//

#include "Core.hpp"

#include <cstring>
#include <sstream>

#include "Base/Base.hpp"
#include "EmulationPlatform/AVR/InstructionExecutor/InstructionExecutor.hpp"

namespace ALinkEmu::AVR {

void Core::Init() {
  // 328p memory stats;
  this->platformDependentData.FLASHEND = 32768;
  this->platformDependentData.RAMEND = 2048;

  this->state = CpuState::BEFORE_INIT;
  this->flash = std::unique_ptr<uint8_t[]>(new uint8_t[this->platformDependentData.FLASHEND + 4]);
  std::memset(this->flash.get(), 0xFF, this->platformDependentData.FLASHEND + 1);
  this->codeEnd = this->platformDependentData.FLASHEND;
  this->ram = std::unique_ptr<uint8_t[]>(new uint8_t[this->platformDependentData.RAMEND + 1]);
  std::memset(this->ram.get(), 0x00, this->platformDependentData.RAMEND);
  this->frequency = 10000000;
  this->instructionExecutor.AttachCore(this);
  this->PC = 0;

  this->ram[0] = 10;
  this->ram[1] = 10;
}

void Core::Reset() {}
void Core::Shutdown() {}
void Core::ExecuteSingleInstruction() {
  uint32_t opcode = this->FetchInstruction(this->PC);
  uint32_t newPC = this->PC + 1;

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
            case 0x0800: {
              this->instructionExecutor.SBC(opcode);
            } break;
            default:
              switch (opcode & 0xFF00) {
                case 0x0100: {
                  this->instructionExecutor.MOVW(opcode);
                }
              }
          }
        }
      }
    }
  }
  // After instruction, it should synchronize SREG with sregMirror

  this->PC = newPC;
  EMU_LOG_INFO("Instruction executed. Core dump: \n{0}", this->DumpCoreData());
}

uint32_t Core::FetchInstruction(FlashAddress currentPC) {
  return this->flash[currentPC] | (this->flash[currentPC + 1] << 8);
}

void Core::LoadFirmware(uint8_t* data, size_t size) { std::memcpy(this->flash.get(), data, size); }

// Simple way to serialize core registers values (especially SREG) for debugging purposes
std::string Core::DumpCoreData() {
  std::string sreg = fmt::format("SREG: I = {0} T = {1}  H = {2} S = {3} V = {4} N = {5} Z = {6} C = {7}\n",
                                 this->GetSregFlagValue(SregFlag::I), this->GetSregFlagValue(SregFlag::T),
                                 this->GetSregFlagValue(SregFlag::H), this->GetSregFlagValue(SregFlag::S),
                                 this->GetSregFlagValue(SregFlag::V), this->GetSregFlagValue(SregFlag::N),
                                 this->GetSregFlagValue(SregFlag::Z), this->GetSregFlagValue(SregFlag::C));

  std::string generalPurposeRegisters;
  for (uint8_t i = 0; i < 32; i++) {
    uint8_t reg = this->GetRegisterValue(i);
    generalPurposeRegisters += fmt::format("r{0}={1} ", i, reg);
  }

  return sreg + generalPurposeRegisters;
}

}  // namespace ALinkEmu::AVR
