//
// Created by Norbert Bielak on 24.11.2021.
//
#include <gtest/gtest.h>

#include <array>

#include "../../ALinkEmu.hpp"

static ALinkEmu::AVR::PlatformData testPlatformData = {
    .RAMEND = 2048,
    .FLASHEND = 32768,
    .E2END = 1024,
};

TEST(AVRInstructionTest, AddFunctionWithOverflowSregTests) {
  using namespace ALinkEmu::AVR;
  AvrChip chip(testPlatformData);
  chip.Init();
  auto& avrCore = chip.core;
  auto& dataMemory = chip.dataMemory;
  auto& programMemory = chip.programMemory;

  uint8_t addOpcode[2] = {
      0x01, 0x0c,  // add r0, r1
  };

  dataMemory.SetRegisterValue(0, 10);
  dataMemory.SetRegisterValue(1, 250);
  programMemory.LoadProgram(addOpcode, 2);
  avrCore.ExecuteSingleInstruction();

  uint8_t result = dataMemory.GetRegisterValue(0);
  EXPECT_EQ(result, 4);

  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::I), false);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::T), false);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::H), true);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::V), false);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::N), false);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::Z), false);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::C), true);
}

TEST(AVRInstructionTest, MULS) {
  using namespace ALinkEmu::AVR;

  AvrChip chip(testPlatformData);
  chip.Init();
  auto& avrCore = chip.core;
  auto& dataMemory = chip.dataMemory;
  auto& programMemory = chip.programMemory;
  uint8_t mulsOpcode[2] = {
      0x45, 0x02  // muls r20, r21
  };

  dataMemory.SetRegisterValue(20, 30);
  dataMemory.SetRegisterValue(21, 30);
  programMemory.LoadProgram(mulsOpcode, 2);
  avrCore.ExecuteSingleInstruction();

  uint8_t R0result = dataMemory.GetRegisterValue(0);
  uint8_t R1result = dataMemory.GetRegisterValue(1);

  EXPECT_EQ(R0result, 0x84);
  EXPECT_EQ(R1result, 0x03);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::C), (R1result >> 7) & 0x01);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::Z), 0);
}

TEST(AVRInstructionTest, MULSU) {
  using namespace ALinkEmu::AVR;
  AvrChip chip(testPlatformData);
  chip.Init();
  auto& avrCore = chip.core;
  auto& dataMemory = chip.dataMemory;
  auto& programMemory = chip.programMemory;

  uint8_t mulsuOpcode[2] = {
      0x45, 0x03  // mulsu r20, r21
  };

  dataMemory.SetRegisterValue(20, 30);
  dataMemory.SetRegisterValue(21, 30);
  programMemory.LoadProgram(mulsuOpcode, 2);
  avrCore.ExecuteSingleInstruction();

  uint8_t R0result = dataMemory.GetRegisterValue(0);
  uint8_t R1result = dataMemory.GetRegisterValue(1);

  EXPECT_EQ(R0result, 0x84);
  EXPECT_EQ(R1result, 0x03);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::C), (R1result >> 7) & 0x01);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::Z), 0);
}

TEST(AVRInstructionTest, FMUL) {
  using namespace ALinkEmu::AVR;
  AvrChip chip(testPlatformData);
  chip.Init();
  auto& avrCore = chip.core;
  auto& dataMemory = chip.dataMemory;
  auto& programMemory = chip.programMemory;

  uint8_t fmulOpcode[2] = {
      0x4D, 0x03  // fmul r20, r21
  };

  dataMemory.SetRegisterValue(20, 30);
  dataMemory.SetRegisterValue(21, 30);
  programMemory.LoadProgram(fmulOpcode, 2);
  avrCore.ExecuteSingleInstruction();

  uint8_t R0result = dataMemory.GetRegisterValue(0);
  uint8_t R1result = dataMemory.GetRegisterValue(1);

  EXPECT_EQ(R0result, 0x08);
  EXPECT_EQ(R1result, 0x07);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::C), (R1result >> 7) & 0x01);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::Z), 0);
}

TEST(AVRInstructionTest, FMULS) {
  using namespace ALinkEmu::AVR;

  AvrChip chip(testPlatformData);
  chip.Init();
  auto& avrCore = chip.core;
  auto& dataMemory = chip.dataMemory;
  auto& programMemory = chip.programMemory;

  uint8_t fmulsOpcode[2] = {
      0xC5, 0x03  // fmuls r20, r21
  };

  dataMemory.SetRegisterValue(20, 30);
  dataMemory.SetRegisterValue(21, 30);
  programMemory.LoadProgram(fmulsOpcode, 2);
  avrCore.ExecuteSingleInstruction();

  uint8_t R0result = dataMemory.GetRegisterValue(0);
  uint8_t R1result = dataMemory.GetRegisterValue(1);

  EXPECT_EQ(R0result, 0x08);
  EXPECT_EQ(R1result, 0x07);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::C), (R1result >> 7) & 0x01);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::Z), 0);
}

TEST(AVRInstructionTest, FMULSU) {
  using namespace ALinkEmu::AVR;
  AvrChip chip(testPlatformData);
  chip.Init();
  auto& avrCore = chip.core;
  auto& dataMemory = chip.dataMemory;
  auto& programMemory = chip.programMemory;

  uint8_t fmulsuOpcode[2] = {
      0xCD, 0x03  // fmulsu r20, r21
  };

  dataMemory.SetRegisterValue(20, 30);
  dataMemory.SetRegisterValue(21, 30);
  programMemory.LoadProgram(fmulsuOpcode, 2);
  avrCore.ExecuteSingleInstruction();

  uint8_t R0result = dataMemory.GetRegisterValue(0);
  uint8_t R1result = dataMemory.GetRegisterValue(1);

  EXPECT_EQ(R0result, 0x08);
  EXPECT_EQ(R1result, 0x07);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::C), (R1result >> 7) & 0x01);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::Z), 0);
}