//
// Created by Norbert Bielak on 18.11.2021.
//

#include "InstructionExecutor.hpp"

#include "EmulationPlatform/AVR/Chip/AvrChip.hpp"
#include "EmulationPlatform/AVR/Core/Core.hpp"

namespace ALinkEmu::AVR {

void InstructionExecutor::CPC(uint32_t opcode) {
  auto [RrAddress, RdAddress] = AddressingModeDecoder::DecodeRr5Rd5(opcode);
  auto& memory = this->coreRef->relatedChip.dataMemory;
  uint8_t Rr = memory.GetRegisterValue(RrAddress);
  uint8_t Rd = memory.GetRegisterValue(RdAddress);

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
  auto& memory = this->coreRef->relatedChip.dataMemory;
  uint8_t Rr = memory.GetRegisterValue(RrAddress);
  uint8_t Rd = memory.GetRegisterValue(RdAddress);
  uint8_t result = Rr + Rd;

  memory.SetRegisterValue(RdAddress, result);

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
  auto& memory = this->coreRef->relatedChip.dataMemory;

  uint8_t Rr = memory.GetRegisterValue(RrAddress);
  uint8_t Rd = memory.GetRegisterValue(RdAddress);
  uint8_t carryFlag = this->coreRef->GetSregFlagValue(SregFlag::C);
  uint8_t result = Rd - Rr - carryFlag;
  memory.SetRegisterValue(RdAddress, result);

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
  auto [RrAddress, RdAddress] = AddressingModeDecoder::DecodeForMOVW(opcode);
  auto& memory = this->coreRef->relatedChip.dataMemory;
  uint8_t Rr = memory.GetRegisterValue(RrAddress);
  uint8_t RrNext = memory.GetRegisterValue(RrAddress + 1);

  memory.SetRegisterValue(RdAddress, Rr);
  memory.SetRegisterValue(RdAddress + 1, RrNext);
}

void InstructionExecutor::MULS(uint32_t opcode) {
  auto [RrAddress, RdAddress] = AddressingModeDecoder::DecodeForMULS(opcode);
  auto& memory = this->coreRef->relatedChip.dataMemory;
  uint8_t Rr = memory.GetRegisterValue(RrAddress);
  uint8_t Rd = memory.GetRegisterValue(RdAddress);

  uint16_t result = Rr * Rd;

  uint8_t R0Value = result & 0x00FF;
  uint8_t R1Value = (result >> 8 & 0x00FF);

  memory.SetRegisterValue(0, R0Value);
  memory.SetRegisterValue(1, R1Value);

  this->coreRef->SetSregFlagValue(SregFlag::C, (result >> 15 & 0x01));
  this->coreRef->SetSregFlagValue(SregFlag::Z, result == 0);
}

void InstructionExecutor::MULSU(uint32_t opcode) {
  auto [RrAddress, RdAddress] = AddressingModeDecoder::DecodeForMUL(opcode);
  auto& memory = this->coreRef->relatedChip.dataMemory;
  uint8_t Rr = memory.GetRegisterValue(RrAddress);
  uint8_t Rd = memory.GetRegisterValue(RdAddress);

  uint16_t result = Rr * Rd;

  uint8_t R0Value = result & 0x00FF;
  uint8_t R1Value = (result >> 8 & 0x00FF);

  memory.SetRegisterValue(0, R0Value);
  memory.SetRegisterValue(1, R1Value);

  this->coreRef->SetSregFlagValue(SregFlag::C, (result >> 15 & 0x01));
  this->coreRef->SetSregFlagValue(SregFlag::Z, result == 0);
}

// performs multiplication and shift one bit to the left
void InstructionExecutor::FMUL(uint32_t opcode) {
  auto [RrAddress, RdAddress] = AddressingModeDecoder::DecodeForMUL(opcode);
  auto& memory = this->coreRef->relatedChip.dataMemory;
  uint8_t Rr = memory.GetRegisterValue(RrAddress);
  uint8_t Rd = memory.GetRegisterValue(RdAddress);

  uint16_t result = (Rr * Rd) << 1;

  uint8_t R0Value = result & 0x00FF;
  uint8_t R1Value = result >> 8 & 0x00FF;

  memory.SetRegisterValue(0, R0Value);
  memory.SetRegisterValue(1, R1Value);

  this->coreRef->SetSregFlagValue(SregFlag::C, (result >> 15 & 0x01));
  this->coreRef->SetSregFlagValue(SregFlag::Z, result == 0);
}

// performs multiplication and shift one bit to the left
void InstructionExecutor::FMULS(uint32_t opcode) {
  auto [RrAddress, RdAddress] = AddressingModeDecoder::DecodeForMUL(opcode);
  auto& memory = this->coreRef->relatedChip.dataMemory;
  uint8_t Rr = memory.GetRegisterValue(RrAddress);
  uint8_t Rd = memory.GetRegisterValue(RdAddress);

  uint16_t result = (Rr * Rd) << 1;

  uint8_t R0Value = result & 0x00FF;
  uint8_t R1Value = result >> 8 & 0x00FF;

  memory.SetRegisterValue(1, R1Value);
  memory.SetRegisterValue(0, R0Value);

  this->coreRef->SetSregFlagValue(SregFlag::C, (result >> 15 & 0x01));
  this->coreRef->SetSregFlagValue(SregFlag::Z, result == 0);
}

// performs multiplication and shift one bit to the left
void InstructionExecutor::FMULSU(uint32_t opcode) {
  auto [RrAddress, RdAddress] = AddressingModeDecoder::DecodeForMUL(opcode);
  auto& memory = this->coreRef->relatedChip.dataMemory;
  uint8_t Rr = memory.GetRegisterValue(RrAddress);
  uint8_t Rd = memory.GetRegisterValue(RdAddress);

  uint16_t result = (Rr * Rd) << 1;

  uint8_t R0Value = result & 0x00FF;
  uint8_t R1Value = result >> 8 & 0x00FF;

  memory.SetRegisterValue(0, R0Value);
  memory.SetRegisterValue(1, R1Value);

  this->coreRef->SetSregFlagValue(SregFlag::C, (result >> 15 & 0x01));
  this->coreRef->SetSregFlagValue(SregFlag::Z, result == 0);
}

void InstructionExecutor::OUT(uint32_t opcode) {
  auto [RrAddress, ioAddress] = AddressingModeDecoder::DecodeR5A6(opcode);
  auto& memory = this->coreRef->relatedChip.dataMemory;
  uint8_t Rr = memory.GetRegisterValue(RrAddress);
  memory.SetRegisterValue(ioAddress, Rr);
}

void InstructionExecutor::IN(uint32_t opcode) {
  auto [RrAddress, ioAddress] = AddressingModeDecoder::DecodeR5A6(opcode);
  auto& memory = this->coreRef->relatedChip.dataMemory;
  uint8_t ioValue = memory.GetRegisterValue(ioAddress);
  memory.SetRegisterValue(RrAddress, ioValue);
}

}  // namespace ALinkEmu::AVR
