#pragma once

#include <queue>
#include <stack>
#include <vector>

#include "Base/Base.hpp"
#include "EmulationPlatform/AVR/Chip/AvrChip.hpp"
#include "EmulationPlatform/AVR/Utils.hpp"

namespace ALinkEmu::AVR {

struct InterruptVector {
  uint8_t vectorNumber;
  RegisterBitLocation isInterruptEnabled;
  RegisterBitLocation isInterruptRaised;
  bool isPending;
  bool isRaiseSticky;
};

struct InterruptTable {
  std::vector<InterruptVector> interruptVectors;
  std::stack<InterruptVector> nestedInterruptVectors;
  std::queue<InterruptVector> pendingInterruptVectors;
};

class InterruptController {
 public:
  explicit InterruptController(AvrChipRef relatedChip) : relatedChip(relatedChip) {}

  inline const InterruptTable& GetInterruptTable() const { return this->interruptTable; }

  void RegisterInterruptVector(const InterruptVector& interruptVector);
  bool RaiseInterrupt(InterruptVector& interruptVector);
  void ClearInterrupt(InterruptVector& interruptVector);
  void ServiceInterrupts();
  void ReturnFromInterrupt();
  bool IsInterruptEnabled() const;
  bool HasPendingInterrupts() const;


 private:
  AvrChipRef relatedChip;
  InterruptTable interruptTable;
};

}  // namespace ALinkEmu::AVR
