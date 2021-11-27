//
// Created by norbert on 27.11.2021.
//

#include "FirmwareLoader.hpp"

#include "Base/Base.hpp"

namespace ALinkEmu::AVR {

struct IntelHexRecordHelper {
  explicit IntelHexRecordHelper(const std::string& intelHexStr) : intelHexStr(intelHexStr) {}

  uint8_t GetNextByte() {
    // oor
    std::string byteView = this->intelHexStr.substr(this->offset, 2);
    uint8_t byte = 0;
    EMU_LOG_INFO("Byte {0}", byteView.data());
    try {
      byte = std::stoul(byteView, nullptr, 16);
      EMU_LOG_INFO(byte);
    } catch (const std::invalid_argument& ex) {
      EMU_LOG_ERROR("Invalid firmware file {0}", ex.what());
    }
    this->offset += 2;
    return byte;
  }

  uint8_t GetNextWord() {
    // oor
    std::string wordView = this->intelHexStr.substr(offset, 4);
    this->offset += 4;
    // catch
    return std::stoi(wordView, nullptr, 16);
  }

 private:
  uint8_t offset = 1;
  const std::string& intelHexStr;
};

FlashImage FirmwareLoader::FromHexFile(std::istream& basicIstream) {
  if (!basicIstream.good()) {
    // invalid stream
  }

  FlashImage flash;

  while (!basicIstream.eof()) {
    std::string hexRecord;
    std::getline(basicIstream, hexRecord);


    if (hexRecord[0] != ':') {
      // InvalidHexFormat
    }
    if (hexRecord.size() <= 5) continue;


    IntelHexRecordHelper helper(hexRecord);
    uint8_t bytesCount = helper.GetNextByte();
    uint16_t address = helper.GetNextWord();
    uint8_t recordType = helper.GetNextByte();

    EMU_LOG_INFO("Bytes count {0} Address {1:x} recordType {2}", bytesCount, address, recordType);
    if (address >= 0x80 && recordType == 0) {
      for (size_t i = 0; i < bytesCount; i++) {
        flash.push_back(helper.GetNextByte());
      }
    }
  }

  return std::move(flash);
}

}  // namespace ALinkEmu::AVR
