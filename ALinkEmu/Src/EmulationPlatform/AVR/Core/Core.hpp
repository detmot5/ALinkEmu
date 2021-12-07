//
// Created by Norbert Bielak on 17.11.2021.
//
#pragma once
#include <memory>
#include <string>

#include "Base/Utils/ArrayBuffer.hpp"
#include "EmulationPlatform/AVR/InstructionExecutor/InstructionExecutor.hpp"
#include "EmulationPlatform/AVR/Utils.hpp"

namespace ALinkEmu::AVR {

enum class CoreState {
  BEFORE_INIT,
  RUNNING,
  STOPPED,
  CRASHED,
};

struct PlatformDependentData {
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

struct IOMemoryAddressListener {
  RamAddress address;
  std::function<void()> write;
  std::function<void()> read;
};

class Core {
  friend class InstructionExecutor;

 public:
  Core() : instructionExecutor(this) {}

  void Init();
  void Reset();
  void Shutdown();
  void ExecuteSingleInstruction();
  uint32_t FetchInstruction(FlashAddress currentPC);
  void LoadFirmware(uint8_t* data, size_t size);

  inline uint8_t GetRegisterValue(RamAddress address) { return this->ram[address]; }
  inline void SetRegisterValue(RamAddress address, uint8_t value) { this->ram[address] = value; }
  inline bool GetSregFlagValue(SregFlag flag) { return this->sregMirror[static_cast<size_t>(flag)]; }
  inline void SetSregFlagValue(SregFlag flag, bool value) {
    this->sregMirror[static_cast<size_t>(flag)] = value;
  }
  inline CoreState GetCoreState() { return this->state; }

  std::string DumpCoreData();

 private:
  InstructionExecutor instructionExecutor;

 private:
  std::string name;
  CoreState state;
  // Program Counter
  FlashAddress PC;
  ByteBuffer ram;
  ByteBuffer flash;
  uint32_t codeEnd;
  uint32_t frequency;

  PlatformDependentData platformDependentData;

  struct MicrocontrollerStatusRegister {
    // Power On Reset Flag
    RegisterBitLocation PORF;
    // External Reset Flag
    RegisterBitLocation EXTRF;
    // Brown Out Reset Flag
    RegisterBitLocation BORF;
    // Watchdog System Reset Flag
    RegisterBitLocation WDRF;
  } MCUSR{};

  bool sregMirror[8] = {false};
};

}  // namespace ALinkEmu::AVR
