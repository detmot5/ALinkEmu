//
// Created by Norbert Bielak on 18.11.2021.
//
#pragma once

#include <cstdint>
#include <utility>

namespace ALinkEmu::AVR {

struct AddressingModeDecoder {
  static inline uint8_t DecodeRd5(uint32_t opcode) { return (opcode >> 4) & 0x0F; }

  // Standard addressing mode for instructions which performs operations on two registers like ADD or CPC
  static inline std::pair<uint8_t, uint8_t> DecodeRr5Rd5(uint32_t opcode) {
    uint8_t RrAddress5 = ((opcode >> 5) & 0x10) | (opcode & 0x0F);
    uint8_t RdAddress5 = (opcode >> 4) & 0x1F;
    return {RrAddress5, RdAddress5};
  }

  /*
   * Special addressing mode where only even register addresses are used
   * That's because usually this addressing mode is used in instructions which
   * performs operations on 16 bit values.
   * For example instruction:
   * "movw r6, r2"  <-> which copies 16 bit value.
   * So r7 and r6 registers stores destination values
   * which is copied from r2 and r3 registers.
   * Register name must be even because after single right bit shift we can store its value on 4 bits
   * (AVR general purpose registers can be stored in 5 bit values (r0 - r31))
   */
  static inline std::pair<uint8_t, uint8_t> DecodeForMOVW(uint32_t opcode) {
    uint8_t RrAddress4 = (opcode & 0x0F) << 1;
    uint8_t RdAddress4 = ((opcode >> 4) & 0x0F) << 1;
    return {RrAddress4, RdAddress4};
  }

  static inline std::pair<uint8_t, uint8_t> DecodeForMULS(uint32_t opcode) {
    uint8_t RrAddress = 16 + (opcode & 0x0F);
    uint8_t RdAddress = 16 + ((opcode >> 4) & 0x0F);
    return {RrAddress, RdAddress};
  }
};

}  // namespace ALinkEmu::AVR
