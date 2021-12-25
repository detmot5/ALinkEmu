//
// Created by Norbert Bielak on 22.12.2021.
//

#include "AvrChip.hpp"

namespace ALinkEmu::AVR {

AvrChip::AvrChip(const PlatformData& platformData)
    : platformData(platformData), core(*this), dataMemory(*this), programMemory(*this) {}

void AvrChip::Init() {
  this->core.Init();
  this->dataMemory.Init();
  this->programMemory.Init();
  for (const auto& ioDevice : this->ioDevices) {
    if (ioDevice) {
      ioDevice->OnInit();
    }
  }
}

void AvrChip::Abort() {
  EMU_LOG_ERROR(
      "Emulated program wanted to perform illegal operation and Abort was called. Core halted, clearing "
      "memory to prevent more damage");
  this->core.Shutdown();
  this->programMemory.Clear();
  this->dataMemory.Clear();
  this->ioController.DetachAllHooks();

  for (const auto& ioDevice : this->ioDevices) {
    if (ioDevice) {
      ioDevice->OnAbort();
    }
  }
}

void AvrChip::Restart() {
  this->core.Restart();
  this->dataMemory.Clear();
  for (auto& ioDevice : this->ioDevices) {
    if (ioDevice) {
      ioDevice->OnRestart();
    }
  }
}
void AvrChip::Run() {
  this->core.ExecuteSingleInstruction();
}

}  // namespace ALinkEmu::AVR
