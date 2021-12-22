//
// Created by Norbert Bielak at 12.12.2021
//
#pragma once

#include "Base/Base.hpp"
#include "Base/Utils/ArrayBuffer.hpp"
#include "EmulationPlatform/AVR/Utils.hpp"
#include "IODeviceBase.hpp"

namespace ALinkEmu::AVR {

class EEPROM : public IODeviceBase {
 public:
  explicit EEPROM(Core* core) : IODeviceBase(core) {}
  void OnAttach() override;
  void OnDetach() override;

 private:
  void OnEECRWrite(uint8_t newEECRvalue);

 private:
  ByteBuffer memory;
  // EEPROM module registers addresses
  RamAddress EEARH;  // EEPROM Address Register High
  RamAddress EEARL;  // EEPROM Address Register Low
  RamAddress EEDR;   // EEPROM Data Register

  RamAddress EECR;               // EEPROM Control Register
  RegisterBitLocation EEMPEBit;  // (ECCR) EEPROM Master Write Enable
  RegisterBitLocation EEPEBit;   // (ECCR) EEPROM Write Enable
  RegisterBitLocation EEREBit;   // (ECCR) EEPROM Read Eanble
};

}  // namespace ALinkEmu::AVR
