#pragma once

#include <Core/BaseChip.hpp>
namespace ALinkEmu {
class ATMega328p : public BaseChip {
 public:
  void Start() override {}
  void Reset() override {}
  void Run() override {}
};
}

