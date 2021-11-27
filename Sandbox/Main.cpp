//
// Created by Norbert Bielak on 03.11.2021.
//

#include <iostream>
#include <QApplication>

#include "ALinkEmu.hpp"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  Logger::Init();
  ALinkEmu::AVR::Core avrCore;
  avrCore.Init();

  // These variables cast to uint8_t* are the same values
  // Since AVR, x86 and any modern CPU are little endian the byte order of multibyte number is reversed
  // So then, array {0x01, 0x0c} in memory looks: 0x0c01
  uint16_t data = 0x0c01;
  uint8_t dataBytes[6] = {
      0x01, 0x04,  // cpc r0, r1
      0x01, 0x0c,  // add r0, r1
      0x10, 0x08   // sbc r1, r0
  };

  avrCore.LoadFirmware(dataBytes, 6);
  avrCore.ExecuteSingleInstruction();
  avrCore.DumpCoreData();
  avrCore.ExecuteSingleInstruction();
  avrCore.DumpCoreData();
  avrCore.ExecuteSingleInstruction();
  avrCore.DumpCoreData();

  return app.exec();
}
