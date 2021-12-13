//
// Created by Norbert Bielak on 24.11.2021.
//
#include <gtest/gtest.h>

#include <array>

#include "../../ALinkEmu.hpp"

TEST(AVRTest, AddFunctionWithOverflowSregTests) {
  using namespace ALinkEmu::AVR;
  ALinkEmu::AVR::Core avrCore;
  Logger::Init();
  avrCore.Init();
  uint8_t addOpcode[2] = {
      0x01, 0x0c,  // add r0, r1
  };

  avrCore.SetRegisterValue(0, 10);
  avrCore.SetRegisterValue(1, 250);
  avrCore.LoadFirmware(addOpcode, 2);
  avrCore.ExecuteSingleInstruction();

  uint8_t result = avrCore.GetRegisterValue(0);
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

  ALinkEmu::AVR::Core avrCore;
  avrCore.Init();
  uint8_t mulsOpcode[2] = {
      0x45, 0x02  // muls r20, r21
  };

  avrCore.SetRegisterValue(20, 30);
  avrCore.SetRegisterValue(21, 30);
  avrCore.LoadFirmware(mulsOpcode, 2);
  avrCore.ExecuteSingleInstruction();

  uint8_t R0result = avrCore.GetRegisterValue(0);
  uint8_t R1result = avrCore.GetRegisterValue(1);

  EXPECT_EQ(R0result, 0x84);
  EXPECT_EQ(R1result, 0x03);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::C), (R1result >> 7) & 0x01);
  EXPECT_EQ(avrCore.GetSregFlagValue(SregFlag::Z), 0);
}