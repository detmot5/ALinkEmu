//
// Created by norbert on 27.11.2021.
//

#include "FirmwareLoader.hpp"

#include "Base/Base.hpp"
#include "Helpers/IntelHexHelpers.hpp"

namespace ALinkEmu::AVR {

FlashImage FirmwareLoader::FromHexFile(std::istream& basicIstream) {
  FlashImage flash;

  std::string hexRecord;
  while (!basicIstream.eof()) {
    if (!basicIstream.good()) {
      throw StandardStreamFailedExeption();
    }
    hexRecord.clear();
    std::getline(basicIstream, hexRecord);

    if (hexRecord.size() > 0) {
      if (hexRecord[0] != ':') {
        throw InvalidIntelHexFileFormatException();
      }

      IntelHexRecordParser parser(hexRecord);
      uint8_t bytesCount = parser.GetNextByte();
      uint16_t address = parser.GetNextWord();
      IntelHexRecordType recordType = static_cast<IntelHexRecordType>(parser.GetNextByte());

      EMU_LOG_INFO("Bytes count {0} Address {1:x} recordType {2}", bytesCount, address, recordType);
      if (address >= INTEL_HEX_AVR_FIRMWARE_START && recordType == IntelHexRecordType::DATA) {
        for (size_t i = 0; i < bytesCount; i++) {
          flash.push_back(parser.GetNextByte());
        }
      }
    } else {
      EMU_LOG_WARN("FirmwareLoader: Detected empty line in firmware file, skipping it");
    }
  }

  return std::move(flash);
}

}  // namespace ALinkEmu::AVR
