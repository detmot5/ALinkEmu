//
// Created by norbert on 18.11.2021.
//

#include "InstructionExecutor.hpp"

#include "EmulationPlatform/AVR/Core/Core.hpp"

namespace ALinkEmu::AVR {

void InstructionExecutor::CPC(uint32_t opcode) {
  auto [RrAddress, RdAddress] = AddressingModeDecoder::DecodeRr5Rd5(opcode);

}

void InstructionExecutor::ADD(uint32_t opcode) {
  auto [RrAddress, RdAddress] = AddressingModeDecoder::DecodeRr5Rd5(opcode);
  uint8_t Rr = this->coreRef->GetRegisterValue(RrAddress);
  uint8_t Rd = this->coreRef->GetRegisterValue(RdAddress);
  uint8_t result = Rr + Rd;
  this->coreRef->SetRegisterValue(RdAddress, result);

  this->coreRef->SetSregFlagValue(SregFlag::Z, result == 0);
  this->coreRef->SetSregFlagValue(SregFlag::N, (result >> 7) & 0x01);

  uint8_t carryFlagsCondition = (Rd & Rr) | (Rr & ~result) | (~result & Rd);
  uint8_t carry = carryFlagsCondition << 7;
  uint8_t halfCarry = carryFlagsCondition << 3;
  this->coreRef->SetSregFlagValue(SregFlag::C, carry & 0x01);
  this->coreRef->SetSregFlagValue(SregFlag::H, halfCarry & 0x01);

  uint8_t twosComplementOverflowFlagCondition = ((Rd & Rr & ~result) | (~Rd, ~Rr, result)) >> 7;
  this->coreRef->SetSregFlagValue(SregFlag::V, twosComplementOverflowFlagCondition & 0x01);

  uint8_t N = this->coreRef->GetSregFlagValue(SregFlag::N);
  uint8_t V = this->coreRef->GetSregFlagValue(SregFlag::V);
  this->coreRef->SetSregFlagValue(SregFlag::S, N ^ V);
}

void InstructionExecutor::SBC(uint32_t opcode) {
  auto [RrAddress, RdAddress] = AddressingModeDecoder::DecodeRr5Rd5(opcode);
  uint8_t Rr = this->coreRef->GetRegisterValue(RrAddress);
  uint8_t Rd = this->coreRef->GetRegisterValue(RdAddress);
  uint8_t result = Rd - Rr;
  // TODO() SREG flags

}
void InstructionExecutor::AttachCore(Core *coreRef)  {
  if (coreRef == nullptr) EMU_LOG_CRITICAL("NULL");
//  EMU_ASSERT(nullptr != coreRef, "InstructionDecoder receiver nullptr Core instance!");
  this->coreRef = coreRef;
}

}

