//
// Created by Norbert Bielak on 24.11.2021.
//
#include <gtest/gtest.h>

#include "../../ALinkEmu.hpp"

ALinkEmu::AVR::Core avrCore;
#include <array>


TEST(AVRTest, AddFunctionWithOverflowSregTests) {
  using namespace ALinkEmu::AVR;
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
