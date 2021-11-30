//
// Created by Norbert Bielak at 30.11.2021
//
#pragma once

#include <exception>

namespace ALinkEmu::AVR {

struct FirmwareLoaderException : public std::exception {
  const char* what() const noexcept override { return "Error occured while loading firmware"; }
};

struct InvalidIntelHexFileFormatException : public FirmwareLoaderException {
  const char* what() const noexcept override {
    return "Error occured while loading firmware - provided Intel Hex file is invalid! ";
  }
};

struct StandardStreamFailedExeption : public FirmwareLoaderException {
  const char* what() const noexcept override {
    return "Error occured while loading firmware - std::stream failed, check file path";
  }
};

}  // namespace ALinkEmu::AVR
