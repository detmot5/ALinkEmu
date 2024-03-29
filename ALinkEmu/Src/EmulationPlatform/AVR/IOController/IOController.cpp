#include "IOController.hpp"

#include <utility>

namespace ALinkEmu::AVR {

void IOController::AttachIoWriteHook(RamAddress address, IOHook ioHook) {
  this->ioWriteHooksTable[address] = std::move(ioHook);
}

void IOController::AttachIoReadHook(RamAddress address, IOHook ioHook) {
  this->ioReadHooksTable[address] = std::move(ioHook);
}

void IOController::DetachIoWriteHook(RamAddress address) { this->ioWriteHooksTable.erase(address); }

void IOController::DetachIoReadHook(RamAddress address) { this->ioReadHooksTable.erase(address); }

void IOController::CallIoWriteHook(RamAddress address) {
  if (this->ioWriteHooksTable.find(address) != this->ioWriteHooksTable.end()) {
    auto hook = this->ioWriteHooksTable[address];
    if (hook) {
      hook(address);
    }
  }
}

void IOController::CallIoReadHook(RamAddress address) {
  if (this->ioReadHooksTable.find(address) != this->ioReadHooksTable.end()) {
    auto hook = this->ioReadHooksTable[address];
    if (hook) {
      hook(address);
    }
  }
}

void IOController::DetachAllHooks() {
  this->ioWriteHooksTable.clear();
  this->ioReadHooksTable.clear();
}

}  // namespace ALinkEmu::AVR
