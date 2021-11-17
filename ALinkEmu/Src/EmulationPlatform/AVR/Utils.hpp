//
// Created by Norbert Bielak on 16.11.2021.
//
#pragma once

#include <stdint.h>

namespace ALinkEmu::AVR {

using RamAddress = uint16_t;
using FlashAddress = uint32_t;

struct RegisterBitLocation {
  RamAddress address;
  uint8_t offset;
};

}
