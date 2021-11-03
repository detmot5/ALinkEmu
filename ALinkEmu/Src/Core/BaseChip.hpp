#pragma once

namespace ALinkEmu {

class BaseChip {
 public:
  virtual void Start() = 0;
  virtual void Reset() = 0;
  virtual void Run() = 0;
};

}  // namespace ALinkEmu
