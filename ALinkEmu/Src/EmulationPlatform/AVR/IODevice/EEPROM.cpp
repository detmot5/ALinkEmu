#include "EEPROM.hpp"

#include "EmulationPlatform/AVR/Core/Core.hpp"

namespace ALinkEmu::AVR {

void EEPROM::OnAttach() {
  auto& ioController = this->coreRef->GetIoController();
  this->memory.Allocate(this->coreRef->GetPlatformDependentData().E2END);
  ioController.AttachIoWriteHook(this->EECR, BIND_MEMBER_FUNCTION_TO_CALLBACK(OnEECRWrite));
}

void EEPROM::OnDetach() {
  auto& ioController = this->coreRef->GetIoController();
  ioController.DetachIoWriteHook(this->EECR);
}

void EEPROM::OnEECRWrite(uint8_t newEECRvalue) {
  EMU_LOG_INFO("EEPROM EECR written!");
  uint16_t eepromAddress = this->EEARL | (this->EEARH << 8);
  bool EEPEValue = this->coreRef->GetRegisterBit(this->EEPEBit);
  bool EEMPEValue = this->coreRef->GetRegisterBit(this->EEMPEBit);
  bool EEREValue = this->coreRef->GetRegisterBit(this->EEREBit);

  // EEMPE should be cleared after 4 cycles but we don't have ability to check it yet

  // we need something like this
  //  auto clearEEMPE = [&]() { this->coreRef->SetRegisterBit(this->EEMPEBit, false); };
  //  this->coreRef.GetSystemClock().attachEventOnCycleCountElapse(4, clearEEMPE);

  if (EEPEValue == true && EEMPEValue == true) {
    this->memory[eepromAddress] = this->coreRef->GetRamValue(this->EEDR);
    this->coreRef->SetRegisterBit(this->EEMPEBit, false);
  }

  if (EEREValue == true) {
    this->coreRef->SetRamValue(this->EEDR, this->memory[eepromAddress]);
  }

  this->coreRef->SetRegisterBit(this->EEPEBit, false);
  this->coreRef->SetRegisterBit(this->EEREBit, false);
}

}  // namespace ALinkEmu::AVR
