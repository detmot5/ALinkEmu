//
// Created by Norbert Bielak on 17.11.2021.
//
#pragma once
#include <memory>
#include <string>

#include "Base/Utils/ArrayBuffer.hpp"
#include "EmulationPlatform/AVR/IOController/IOController.hpp"
#include "EmulationPlatform/AVR/InstructionExecutor/InstructionExecutor.hpp"
#include "EmulationPlatform/AVR/Utils.hpp"

namespace ALinkEmu::AVR {

enum class CoreState {
  BEFORE_INIT,
  RUNNING,
  STOPPED,
  CRASHED,
};

struct AvrChip;

class Core {
  friend class InstructionExecutor;

 public:
  // TODO: Clang-Tidy: Constructor does not initialize these fields: state, PC, codeEnd, frequency
  explicit Core(AvrChip& relatedChip) : instructionExecutor(this), relatedChip(relatedChip) {}

  void Init();
  void Restart();
  void Shutdown();
  void ExecuteSingleInstruction();

  inline bool GetSregFlagValue(SregFlag flag) { return this->sregMirror[static_cast<size_t>(flag)]; }
  inline void SetSregFlagValue(SregFlag flag, bool value) {
    this->sregMirror[static_cast<size_t>(flag)] = value;
  }

  inline CoreState GetCoreState() { return this->state; }

  std::string DumpCoreData();

 private:
  InstructionExecutor instructionExecutor;

 private:
  AvrChip& relatedChip;
  std::string name;
  CoreState state;
  FlashAddress PC;  // Program Counter
  uint32_t codeEnd;
  uint32_t frequency;

  struct MicrocontrollerStatusRegister {
    // Power On Restart Flag
    RegisterBitLocation PORF;
    // External Restart Flag
    RegisterBitLocation EXTRF;
    // Brown Out Restart Flag
    RegisterBitLocation BORF;
    // Watchdog System Restart Flag
    RegisterBitLocation WDRF;
  } MCUSR{};

  bool sregMirror[8] = {false};
};

}  // namespace ALinkEmu::AVR
