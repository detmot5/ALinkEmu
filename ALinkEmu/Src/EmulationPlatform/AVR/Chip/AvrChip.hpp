//
// Created by Norbert Bielak on 22.12.2021.
//
#pragma once

#include <memory>
#include <vector>

#include "Base/Base.hpp"
#include "EmulationPlatform/AVR/Core/Core.hpp"
#include "EmulationPlatform/AVR/IOController/IOController.hpp"
#include "EmulationPlatform/AVR/IODevice/IODevice.hpp"
#include "EmulationPlatform/AVR/Memory/DataMemory.hpp"
#include "EmulationPlatform/AVR/Memory/ProgramMemory.hpp"

namespace ALinkEmu::AVR {

struct PlatformData {
  // Last address of IO Section
  uint16_t IOEND;
  // Last address of RAM
  uint16_t RAMEND;
  // Last address of Flash memory
  uint32_t FLASHEND;
  // Last Address of EEPROM memory
  uint32_t E2END;
  uint16_t RAMPZ;  // optional, only for ELPM/SPM on >64Kb cores
  uint16_t EIND;   // optional, only for EIJMP/RECALL on >64Kb cores
  uint8_t vectorSize;
  uint8_t signature[3];
  uint8_t fuse[6];
  uint8_t lockbits;
  uint8_t addressSize;  // 2, or 3 for cores > 128KB in flash
};


struct AvrChip {
  AvrChip(const PlatformData& platformData);

  void Init();
  void Restart();
  void Abort();
  void Run();

  PlatformData platformData;
  Core core;
  DataMemory dataMemory;
  ProgramMemory programMemory;
  IOController ioController;
  std::vector<std::unique_ptr<IODevice>> ioDevices;
};


using AvrChipRef = AvrChip&;

}  // namespace ALinkEmu::AVR
