//
// Created by Norbert Bielak on 17.11.2021.
//

#include "Core.hpp"

#include "Base/Base.hpp"
#include "Base/Utils/ArrayBuffer.hpp"
#include "EmulationPlatform/AVR/Chip/AvrChip.hpp"

namespace ALinkEmu::AVR {

void Core::Init() {
  // 328p memory stats;
  this->relatedChip.platformData.FLASHEND = 32768;
  this->relatedChip.platformData.RAMEND = 2048;

  this->state = CoreState::BEFORE_INIT;
  this->codeEnd = this->relatedChip.platformData.FLASHEND;
  this->frequency = 10000000;
  this->PC = 0;

  this->state = CoreState::RUNNING;
}

void Core::Restart() {
  this->PC = 0;
}

void Core::Shutdown() {
  this->PC = 0;
  this->state = CoreState::STOPPED;
}

void Core::ExecuteSingleInstruction() {
  auto& programMemory = this->relatedChip.programMemory;
  uint32_t opcode = programMemory.FetchInstruction(this->PC);
  uint32_t newPC = this->PC + 2;

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
                } break;
                case 0x0200: {
                  this->instructionExecutor.MULS(opcode);
                } break;
                case 0x0300: {
                  switch (opcode & 0x88) {
                    case 0x00: {
                      this->instructionExecutor.MULSU(opcode);
                    } break;
                    case 0x08: {
                      this->instructionExecutor.FMUL(opcode);
                    } break;
                    case 0x80: {
                      this->instructionExecutor.FMULS(opcode);
                    } break;
                    case 0x88: {
                      this->instructionExecutor.FMULSU(opcode);
                    } break;
                  }
                }
              }
          }
        }
      }
    } break;

    case 0x1000: {
      switch(opcode & 0xfc00) {
        case 0x1800: {
          this->instructionExecutor.SUB(opcode);
        }
      }
    }

    case 0x9000: {
      case 0xB000: {
        switch (opcode & 0xF800) {
          case 0xB800: {
            this->instructionExecutor.OUT(opcode);
          } break;
          case 0xB000: {
            this->instructionExecutor.IN(opcode);
          } break;
        }
      } break;
    } break;
    default:
      EMU_LOG_WARN("Invalid opcode: {0:x}", opcode);
  }
  // After instruction, it should synchronize SREG with sregMirror

  this->PC = newPC;
  EMU_LOG_INFO("Instruction executed. Core dump: \n{0}", this->DumpCoreData());
}


std::string Core::DumpCoreData() {
  // Simple way to serialize core registers values (especially SREG) for debugging purposes
  std::string sreg = fmt::format("SREG: I = {0} T = {1}  H = {2} S = {3} V = {4} N = {5} Z = {6} C = {7}\n",
                                 this->GetSregFlagValue(SregFlag::I), this->GetSregFlagValue(SregFlag::T),
                                 this->GetSregFlagValue(SregFlag::H), this->GetSregFlagValue(SregFlag::S),
                                 this->GetSregFlagValue(SregFlag::V), this->GetSregFlagValue(SregFlag::N),
                                 this->GetSregFlagValue(SregFlag::Z), this->GetSregFlagValue(SregFlag::C));

  std::string generalPurposeRegisters;
  for (uint8_t i = 0; i < 32; i++) {
    uint8_t reg = this->relatedChip.dataMemory.GetRegisterValue(i);
    generalPurposeRegisters += fmt::format("r{0}={1} ", i, reg);
  }

  return sreg + generalPurposeRegisters;
}

}  // namespace ALinkEmu::AVR
