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
  uint8_t data[2] = {0x0c, 0x01};
  avrCore.LoadFirmware(data, 2);
  avrCore.ExecuteSingleInstruction();




  return app.exec();
}
