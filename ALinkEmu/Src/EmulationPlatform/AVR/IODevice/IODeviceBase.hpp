//
// Created by Norbert Bielak at 02.12.2021
//
#pragma once

#include <functional>
#include <string>

#include "Base/Base.hpp"

namespace ALinkEmu::AVR {

class Core;

class IODeviceBase {
 public:
  explicit IODeviceBase(Core* coreRef) : coreRef(coreRef) {}
  // IO module will detect when it's memory was modified by program
  // and call proper function just after writing it to memory
  // it's not necessary to all memory registers but only that where bit change should fire some action
  // Like rising write flag in EEPROM Control Register should fire writing data from EEPROM data register
  // So, when you writing IO device implmentation you can write your own events which will be called
  // when provided register will be written or accessed
  virtual void OnAttach();
  virtual void OnDetach();

 protected:
  Core* coreRef;
  std::string name;
};

}  // namespace ALinkEmu::AVR
