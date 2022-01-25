//
// Created by Norbert Bielak on 16.11.2021.
//
#pragma once

#include <cstdint>

namespace ALinkEmu::AVR {

using RamAddress = uint16_t;
using FlashAddress = uint32_t;

struct RegisterBitLocation {
  RamAddress address;
  uint8_t offset;
};

enum class SregFlag : uint8_t {
  C = 0,
  Z = 1,
  N = 2,
  V = 3,
  S = 4,
  H = 5,
  T = 6,
  I = 7,
};

}  // namespace ALinkEmu::AVR
