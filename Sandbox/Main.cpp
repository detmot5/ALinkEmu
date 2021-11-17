//
// Created by Norbert Bielak on 03.11.2021.
//

#include <iostream>
#include <QApplication>

#include "ALinkEmu.hpp"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  Logger::Init();
  ALinkEmu::ATMega328p chip;
  chip.Start();
  chip.Run();
  std::cout << "Start..." << std::endl;
  return app.exec();
}
