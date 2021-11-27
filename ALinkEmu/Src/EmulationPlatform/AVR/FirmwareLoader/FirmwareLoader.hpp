//
// Created by Norbert Bielak on 27.11.2021.
//
#pragma once
#include <iostream>
#include <memory>
#include <vector>

namespace ALinkEmu::AVR {

using FlashImage = std::vector<uint8_t>;

class FirmwareLoader {
 public:
  FlashImage FromHexFile(std::istream& basicIstream);
};

}  // namespace ALinkEmu::AVR
