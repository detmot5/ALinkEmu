//
// Created by norbert on 18.11.2021.
//

#include "InstructionExecutor.hpp"

#include <elf.h>

#include "EmulationPlatform/AVR/Core/Core.hpp"

namespace ALinkEmu::AVR {

void InstructionExecutor::CPC(uint32_t opcode) {
  auto [RrAddress, RdAddress] = AddressingModeDecoder::DecodeRr5Rd5(opcode);
  uint8_t Rr = this->coreRef->GetRegisterValue(RrAddress);
  uint8_t Rd = this->coreRef->GetRegisterValue(RdAddress);

  uint8_t carryFlag = this->coreRef->GetSregFlagValue(SregFlag::C);

  uint8_t result = Rd - Rr - carryFlag;

  uint8_t carryFlagsCondition = (~Rd & Rr) | (Rr & result) | (result & ~Rd);
  uint8_t carry = (carryFlagsCondition >> 7) & 0x01;
  uint8_t halfCarry = (carryFlagsCondition >> 3) & 0x01;

  this->coreRef->SetSregFlagValue(SregFlag::C, carry);
  this->coreRef->SetSregFlagValue(SregFlag::H, halfCarry);

  uint8_t previousZeroFlag = this->coreRef->GetSregFlagValue(SregFlag::Z);
  uint8_t zeroFlag = (~result) & previousZeroFlag;
  this->coreRef->SetSregFlagValue(SregFlag::Z, zeroFlag);

  uint8_t negativeFlag = (result >> 7) & 0x01;
  uint8_t twosComplementFlag = (((Rd & ~Rr & ~result) | (~Rd & Rr & result)) >> 7) & 0x01;
  this->coreRef->SetSregFlagValue(SregFlag::N, negativeFlag);
  this->coreRef->SetSregFlagValue(SregFlag::V, twosComplementFlag);

  uint8_t signFlag = negativeFlag ^ twosComplementFlag;
  this->coreRef->SetSregFlagValue(SregFlag::S, signFlag);
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
  uint8_t carry = carryFlagsCondition >> 7;
  uint8_t halfCarry = carryFlagsCondition >> 3;
  this->coreRef->SetSregFlagValue(SregFlag::C, carry & 0x01);
  this->coreRef->SetSregFlagValue(SregFlag::H, halfCarry & 0x01);

  uint8_t twosComplementOverflowFlagCondition = (((Rd & Rr & ~result) | (~Rd & ~Rr & result)) >> 7) & 0x01;
  this->coreRef->SetSregFlagValue(SregFlag::V, twosComplementOverflowFlagCondition);

  uint8_t N = this->coreRef->GetSregFlagValue(SregFlag::N);
  uint8_t V = this->coreRef->GetSregFlagValue(SregFlag::V);
  this->coreRef->SetSregFlagValue(SregFlag::S, N ^ V);
}

void InstructionExecutor::SBC(uint32_t opcode) {
  auto [RrAddress, RdAddress] = AddressingModeDecoder::DecodeRr5Rd5(opcode);
  uint8_t Rr = this->coreRef->GetRegisterValue(RrAddress);
  uint8_t Rd = this->coreRef->GetRegisterValue(RdAddress);
  uint8_t carryFlag = this->coreRef->GetSregFlagValue(SregFlag::C);
  uint8_t result = Rd - Rr - carryFlag;
  this->coreRef->SetRegisterValue(RdAddress, result);

  uint8_t carryFlagsCondition = (~Rd & Rr) | (Rr & result) | (result & ~Rd);
  uint8_t carry = (carryFlagsCondition >> 7) & 0x01;
  uint8_t halfCarry = (carryFlagsCondition >> 3) & 0x01;

  this->coreRef->SetSregFlagValue(SregFlag::C, carry);
  this->coreRef->SetSregFlagValue(SregFlag::H, halfCarry);

  uint8_t previousZeroFlag = this->coreRef->GetSregFlagValue(SregFlag::Z);
  uint8_t zeroFlag = (~result) & previousZeroFlag;
  this->coreRef->SetSregFlagValue(SregFlag::Z, zeroFlag);

  uint8_t negativeFlag = (result >> 7) & 0x01;
  uint8_t twosComplementOverflowFlag = (((Rd & ~Rr & ~result) | (~Rd & Rr & result)) >> 7) & 0x01;
  this->coreRef->SetSregFlagValue(SregFlag::N, negativeFlag);
  this->coreRef->SetSregFlagValue(SregFlag::V, twosComplementOverflowFlag);

  uint8_t signFlag = negativeFlag ^ twosComplementOverflowFlag;
  this->coreRef->SetSregFlagValue(SregFlag::S, signFlag);
}

void InstructionExecutor::MOVW(uint32_t opcode) {
  auto [RrAddress, RdAddress] = AddressingModeDecoder::DecodeRr4Rd4(opcode);
  uint8_t Rr = this->coreRef->GetRegisterValue(RrAddress);
  uint8_t RrNext = this->coreRef->GetRegisterValue(RrAddress + 1);

  this->coreRef->SetRegisterValue(RdAddress, Rr);
  this->coreRef->SetRegisterValue(RdAddress + 1, RrNext);
}

}  // namespace ALinkEmu::AVR
