//
// Created by Norbert Bielak at 28.11.2021
//
#pragma once

#include <iostream>

#include "Base/Base.hpp"

namespace ALinkEmu::AVR {

enum class IntelHexRecordType : uint8_t {
  DATA = 0x00,
  END_OF_LINE = 0x01,
  EXTENDED_SEGMENT_ADDRESS = 0x02,
  START_SEGMENT_ADDRESS = 0x03,
  EXTENDED_LINEAR_ADDRESS = 0x04,
  START_LINEAR_ADDRESS = 0x05
};

#define INTEL_HEX_AVR_FIRMWARE_START 0x80

/*
 * Simple class which helps to extract bytes and words from intel hex file
 * Should be stack allocated and re-created for every record (due to "offset" variable)
 * Allocating it is not huge deal and it's more readable than adding some Restart() function which would set
 * offset back to 1
 */
class IntelHexRecordParser {
 public:
  explicit IntelHexRecordParser(const std::string& intelHexRecordRef)
      : intelHexRecordRef(intelHexRecordRef) {}

  uint8_t GetNextByte() {
    std::string wordSubstr = this->intelHexRecordRef.substr(this->offset, 2);
    uint8_t byte = 0;
    EMU_LOG_INFO("Byte {0}", wordSubstr);
    try {
      byte = std::stoul(wordSubstr, nullptr, 16);
      EMU_LOG_INFO(byte);
    } catch (const std::invalid_argument& ex) {
      EMU_LOG_ERROR("Invalid firmware file {0}", ex.what());
    }
    this->offset += 2;
    return byte;
  }

  uint16_t GetNextWord() {
    std::string wordSubstr = this->intelHexRecordRef.substr(this->offset, 4);
    uint16_t word = 0;
    try {
      word = std::stoul(wordSubstr, nullptr, 16);
    } catch (const std::invalid_argument& ex) {
      EMU_LOG_ERROR("Invalid value in provided Intel Hex file!");
    }
    return word;
  }

 private:
  // At the beginning of intel hex file record is ":", we need to skip it so that's why offset is 1
  uint8_t offset = 1;
  const std::string& intelHexRecordRef;
};

}  // namespace ALinkEmu::AVR
