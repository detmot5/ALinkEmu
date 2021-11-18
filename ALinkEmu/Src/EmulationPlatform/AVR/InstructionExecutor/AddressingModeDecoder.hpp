//
// Created by Norbert Bielak on 18.11.2021.
//
#pragma once

#include <cstdint>
#include <utility>

namespace ALinkEmu::AVR {

struct AddressingModeDecoder {

  static inline uint8_t DecodeRd5(uint32_t opcode) {
    return (opcode >> 4) & 0xf;
  }
  static inline std::pair<uint8_t, uint8_t> DecodeRr5Rd5(uint32_t opcode) {
    uint8_t RrAddress5 = ((opcode >> 5) & 0x10) | (opcode & 0xf);
    uint8_t RdAddress5 = (opcode >> 4) & 0x1F;
    return {RrAddress5, RdAddress5};
  }
};


}
