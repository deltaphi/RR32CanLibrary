#include "StructuredConfigDataCallback.h"

#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace udp_station {
namespace {

std::string quoteToml(const RR32Can::BufferManager& text) {
  static const char hex[] = "0123456789ABCDEF";
  std::string result = "\"";
  for (size_t i = 0; i < text.length(); ++i) {
    const auto c = static_cast<unsigned char>(text.data()[i]);
    if (c == '"' || c == '\\') {
      result += '\\';
      result += static_cast<char>(c);
    } else if (c < 0x20 || c == 0x7f) {
      result += "\\u00";
      result += hex[c >> 4];
      result += hex[c & 0xf];
    } else {
      result += static_cast<char>(c);
    }
  }
  result += '"';
  return result;
}

}  // namespace

StructuredConfigDataCallback::StructuredConfigDataCallback(const std::string& filename) : filename_(filename) {
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

void StructuredConfigDataCallback::consumeConfigData(RR32Can::BufferManager& section, RR32Can::BufferManager& key,
                                                     RR32Can::BufferManager& value) {
  writeText("[[entries]]\nsection = " + quoteToml(section) + "\nkey = " + quoteToml(key) +
            "\nvalue = " + quoteToml(value) + "\n\n");
}

void StructuredConfigDataCallback::setStreamComplete() {
  if (file_ && std::fflush(file_.get()) != 0) {
    throw std::runtime_error("Cannot write output file '" + filename_ + "': " + std::strerror(errno));
  }
}

void StructuredConfigDataCallback::setStreamAborted() {}

void StructuredConfigDataCallback::closeFile() { file_.reset(); }

void StructuredConfigDataCallback::writeText(const std::string& text) {
  if (!file_) {
    std::cout << text;
    return;
  }

  if (std::fwrite(text.data(), 1, text.size(), file_.get()) != text.size()) {
    throw std::runtime_error("Cannot write output file '" + filename_ + "': " + std::strerror(errno));
  }
}

}  // namespace udp_station
