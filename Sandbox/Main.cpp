//
// Created by Norbert Bielak on 03.11.2021.
//

#include <QApplication>
#include <iostream>
#include <sstream>

#include "ALinkEmu.hpp"

std::string hexfile = R"(
:100000000C9434000C943E000C943E000C943E0082
:100010000C943E000C943E000C943E000C943E0068
:100020000C943E000C943E000C943E000C943E0058
:100030000C943E000C943E000C943E000C943E0048
:100040000C943E000C943E000C943E000C943E0038
:100050000C943E000C943E000C943E000C943E0028
:100060000C943E000C943E0011241FBECFEFD8E04C
:10007000DEBFCDBF0E9440000C9461000C940000D4
:10008000010C010C010C010C010C010C010C010C08
:10009000010C010C010C010C010C010C010C010CF8
:1000A000010C010C010C010C010C010C010C010CE8
:1000B000010C010C010C010C010C010C010C010CD8
:0600C000010CF894FFCFD3
:00000001FF
)";

ALinkEmu::AVR::PlatformData data = {
    .RAMEND = 2048,
    .FLASHEND = 32768,
    .E2END = 1024,
};

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  Logger::Init();


  ALinkEmu::AVR::AvrChip chip(data);
  ALinkEmu::AVR::FirmwareLoader loader;

  chip.Init();

  chip.dataMemory.SetValueInMemory(0, 10);
  chip.dataMemory.SetValueInMemory(1, 10);

  std::istringstream hexFileStream(hexfile);
  try {
    ALinkEmu::AVR::FlashImage image = loader.FromHexFile(hexFileStream);
    chip.programMemory.LoadProgram(image.data(), image.size());
  } catch (ALinkEmu::AVR::FirmwareLoaderException& ex) {
    EMU_CLIENT_LOG_ERROR("Firmware loading failed: {0}", ex.what());
  }

  chip.Run();
  chip.Run();
  chip.Run();

  //  // These variables cast to uint8_t* are the same values
  //  // Since AVR, x86 and any modern CPU are little endian the byte order of multibyte number is reversed
  //  // So then, array {0x01, 0x0c} in memory looks: 0x0c01
  //  uint16_t data = 0x0c01;
  //  uint8_t dataBytes[6] = {
  //      0x01, 0x04,  // cpc r0, r1
  //      0x01, 0x0c,  // add r0, r1
  //      0x10, 0x08   // sbc r1, r0
  //  };

  return app.exec();
}
