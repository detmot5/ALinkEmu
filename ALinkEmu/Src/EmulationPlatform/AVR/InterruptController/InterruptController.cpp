#include "InterruptController.hpp"

namespace ALinkEmu::AVR {

void InterruptController::RegisterInterruptVector(const InterruptVector& interruptVector) {
  interruptTable.interruptVectors.push_back(interruptVector);
}

bool InterruptController::RaiseInterrupt(InterruptVector& interruptVector) {
  if (interruptVector.vectorNumber == 0) return false;
  auto& dataMemory = this->relatedChip.dataMemory;

  dataMemory.SetRegisterBit(interruptVector.isInterruptRaised);

  bool isInterruptEnabled = dataMemory.GetRegisterBit(interruptVector.isInterruptEnabled);
  if (isInterruptEnabled) {
    interruptVector.isPending = true;
    this->interruptTable.pendingInterruptVectors.emplace(interruptVector);
    //  if global interrupts are enabled then wake up cpu

  }
  return true;
}
void InterruptController::ClearInterrupt(InterruptVector& interruptVector) {
    interruptVector.isPending = false;
    if (!interruptVector.isRaiseSticky) {
      this->relatedChip.dataMemory.ClearRegisterBit(interruptVector.isInterruptRaised);
    }
}

void InterruptController::ServiceInterrupts() {}
void InterruptController::ReturnFromInterrupt() {}
bool InterruptController::IsInterruptEnabled() const { return false; }
bool InterruptController::HasPendingInterrupts() const { return false; }

}  // namespace ALinkEmu::AVR
