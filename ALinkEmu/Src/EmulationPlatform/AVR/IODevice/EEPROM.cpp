#include "EEPROM.hpp"

#include "EmulationPlatform/AVR/Core/Core.hpp"
#include "EmulationPlatform/AVR/Chip/AvrChip.hpp"

namespace ALinkEmu::AVR {

void EEPROM::OnInit() {
  auto& ioController = this->relatedChip.ioController;
  this->memory.Allocate(this->relatedChip.platformData.E2END);
  ioController.AttachIoWriteHook(this->EECR, BIND_MEMBER_FUNCTION_TO_CALLBACK(OnEECRWrite));
}

void EEPROM::OnShutdown() {
  auto& ioController = this->relatedChip.ioController;
  ioController.DetachIoWriteHook(this->EECR);
}

void EEPROM::OnEECRWrite(uint8_t newEECRvalue) {
  EMU_LOG_INFO("EEPROM EECR written!");
  auto& dataMemory = this->relatedChip.dataMemory;
  uint16_t eepromAddress = this->EEARL | (this->EEARH << 8);
  bool EEPEValue = dataMemory.GetRegisterBit(this->EEPEBit);
  bool EEMPEValue = dataMemory.GetRegisterBit(this->EEMPEBit);
  bool EEREValue = dataMemory.GetRegisterBit(this->EEREBit);


  // EEMPE should be cleared after 4 cycles but we don't have ability to check it yet
  // we need something like this
  //  auto clearEEMPE = [&]() { this->coreRef->SetRegisterBit(this->EEMPEBit, false); };
  //  this->coreRef.GetSystemClock().AttachEventOnCycleCountElapse(4, clearEEMPE);

  if (EEPEValue == true && EEMPEValue == true) {
    this->memory[eepromAddress] = dataMemory.GetValueFromMemory(this->EEDR);
    dataMemory.ClearRegisterBit(this->EEMPEBit);
  }

  if (EEREValue == true) {
    dataMemory.SetValueInMemory(this->EEDR, this->memory[eepromAddress]);
  }

  dataMemory.ClearRegisterBit(this->EEPEBit);
  dataMemory.ClearRegisterBit(this->EEREBit);
}

}  // namespace ALinkEmu::AVR
