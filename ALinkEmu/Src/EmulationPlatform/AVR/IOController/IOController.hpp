//
// Created by Norbert Bielak at 12.12.2021
//
#pragma once

#include <functional>
#include <unordered_map>

#include "EmulationPlatform/AVR/Utils.hpp"

namespace ALinkEmu::AVR {

class Core;

using IOHook = std::function<void(uint8_t)>;

class IOController {
 public:
  explicit IOController(Core* coreRef) : coreRef(coreRef) {}

  void AttachIoWriteHook(RamAddress address, IOHook ioHook);
  void AttachIoReadHook(RamAddress address, IOHook ioHook);

  void DetachIoWriteHook(RamAddress address);
  void DetachIoReadHook(RamAddress address);

  void CallIoWriteHook(RamAddress address);
  void CallIoReadHook(RamAddress address);

 private:
  Core* coreRef;
  // Using hash tables instead of normal arrays and trying to map ram addresses to it's indexes for better
  // readability. This should not have any significant performance overhead since key is uint16_t and there
  // should not be any the same hashes -> so it should act as normal array
  std::unordered_map<RamAddress, IOHook> ioWriteHooksTable;
  std::unordered_map<RamAddress, IOHook> ioReadHooksTable;
};

}  // namespace ALinkEmu::AVR
