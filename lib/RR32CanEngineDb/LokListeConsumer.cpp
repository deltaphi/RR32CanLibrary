#include "RR32CanEngineDb/LokListeConsumer.h"

namespace RR32Can {
namespace {
bool equals(const BufferManager& text, const char* expected) {
  return text.length() == strlen(expected) && memcmp(text.data(), expected, text.length()) == 0;
}

bool readNumber(const BufferManager& text,
                uint32_t& result) {  // TODO: Could this be some library function? Move to an arduino adapter?
  if (text.empty()) {
    return false;
  } else {
    result = 0;
    for (uint8_t i = 0; i < text.length(); ++i) {
      const char c = text[i];
      if (c < '0' || c > '9' || result > (UINT32_MAX - (c - '0')) / 10) return false;
      result = result * 10 + (c - '0');
    }
    return true;
  }
}
}  // namespace

void LokListeConsumer::clear() {
  currentLocomotive_ = nullptr;
  complete_ = false;
  invalid_ = false;
}

void LokListeConsumer::consumeConfigData(BufferManager& section, BufferManager& key, BufferManager& value) {
  if (equals(section, "ERROR")) invalid_ = true;
  if (!equals(section, "lokliste")) return;
  if (equals(key, "idx")) {
    uint32_t index = 0;
    if (!readNumber(value, index)) {
      currentLocomotive_ = nullptr;
      invalid_ = true;
      return;
    }
    currentLocomotive_ = callback_.getLokInfoShort(index);
    if (currentLocomotive_ != nullptr) {
      currentLocomotive_->reset();
    }
  } else {
    // These fields are not part of LocomotiveShortInfo, but malformed numeric
    // values must still invalidate the stream when the record is accepted.
    uint32_t ignored = 0;
    if (equals(key, "llindex") || equals(key, "crc")) {
      if (!readNumber(value, ignored)) {
        invalid_ = true;
      }
    }
    if (currentLocomotive_ == nullptr) {
      return;
    }
    if (equals(key, "name")) {
      if (!value.empty()) {
        currentLocomotive_->setName(value.data());
      }
    }
  }
}

void LokListeConsumer::setStreamComplete() {
  if (invalid_) {
    setStreamAborted();
  } else {
    complete_ = true;
    if (streamEndCallback_ != nullptr) {
      streamEndCallback_->streamComplete(this);
    }
  }
}

void LokListeConsumer::setStreamAborted() {
  clear();
  if (streamEndCallback_ != nullptr) {
    streamEndCallback_->streamAborted(this);
  }
}

}  // namespace RR32Can
