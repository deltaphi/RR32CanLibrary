#include "TextConfigDataCallback.h"

#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace udp_station {

TextConfigDataCallback::TextConfigDataCallback(const std::string& filename, FileMode fileMode)
    : filename_(filename), fileMode_(fileMode) {
  if (!filename_.empty()) {
    // Exclusive creation avoids overwriting a file even if another process creates it concurrently.
    file_.reset(std::fopen(filename_.c_str(), "wbx"));
    if (!file_) {
      const int error = errno;
      if (error == EEXIST) {
        throw std::runtime_error("Output file '" + filename_ + "' already exists; refusing to overwrite it");
      }
      throw std::runtime_error("Cannot create output file '" + filename_ + "': " + std::strerror(error));
    }
  }
}

void TextConfigDataCallback::startStream() { streamState = StreamState::WAITING_FIRST_PACKET; }

void TextConfigDataCallback::addMessage(const RR32Can::Data& data) {
  if (file_) {
    size_t bytesToWrite = 0;
    size_t bytesWritten = 0;
    for (uint8_t i = 0; i < data.dlc; ++i) {
      if (fileMode_ == FileMode::OmitNullCharacters && data.data[i] == 0) {
        continue;
      }
      ++bytesToWrite;
      bytesWritten += std::fwrite(&data.data[i], 1, 1, file_.get());
    }
    if (bytesWritten != bytesToWrite || std::fflush(file_.get()) != 0) {
      throw std::runtime_error("Cannot write output file '" + filename_ + "': " + std::strerror(errno));
    }
    return;
  }
  data.printAsText();
  std::cout << '\n';
}

void TextConfigDataCallback::closeFile() { file_.reset(); }

void TextConfigDataCallback::reset() { streamState = StreamState::IDLE; }

}  // namespace udp_station
