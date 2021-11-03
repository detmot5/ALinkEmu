//
// Created by Norbert Bielak on 03.11.2021.
//

#include <iostream>

#include "ALinkEmu.hpp"

int main() {
  ALinkEmu::ATMega328p chip;
  chip.Start();
  chip.Run();
  std::cout << "Start..." << std::endl;
}
