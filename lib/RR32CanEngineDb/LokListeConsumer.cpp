#include "RR32CanEngineDb/LokListeConsumer.h"

#include "RR32Can/Station.h"

namespace RR32Can {
namespace {
bool equals(const BufferManager& text, const char* expected) {
  return text.length() == strlen(expected) && memcmp(text.data(), expected, text.length()) == 0;
}

bool readNumber(const BufferManager& text, uint32_t& result) {
  if (text.empty()) return false;
  result = 0;
  for (uint8_t i = 0; i < text.length(); ++i) {
    const char c = text[i];
    if (c < '0' || c > '9' || result > (UINT32_MAX - (c - '0')) / 10) return false;
    result = result * 10 + (c - '0');
  }
  return true;
}
}  // namespace

LokListeConsumer::LokListeConsumer(Entry* storage, size_t capacity)
    : storage_(storage), capacity_(storage == nullptr ? 0 : capacity) {
  clear();
}

void LokListeConsumer::clear() {
  for (size_t i = 0; i < capacity_; ++i) storage_[i] = Entry{};
  pending_ = Entry{};
  occupiedCount_ = 0;
  haveIndex_ = complete_ = overflow_ = invalid_ = false;
}

void LokListeConsumer::download(Station& station) {
  station.AbortCurrentConfigRequest();
  parser_.reset();
  clear();
  parser_.startStream(this);
  station.RequestLokListe(&parser_);
}

void LokListeConsumer::finishEntry() {
  if (!haveIndex_) return;
  if (pending_.locomotive.isNameKnown()) ++occupiedCount_;
  if (index_ < capacity_) {
    storage_[index_] = pending_;
  } else {
    overflow_ = true;
  }
  haveIndex_ = false;
}

void LokListeConsumer::consumeConfigData(BufferManager& section, BufferManager& key, BufferManager& value) {
  if (equals(section, "ERROR")) invalid_ = true;
  if (!equals(section, "lokliste")) return;
  if (equals(key, "idx")) {
    finishEntry();
    pending_ = Entry{};
    haveIndex_ = readNumber(value, index_);
    if (!haveIndex_) invalid_ = true;
  } else if (haveIndex_) {
    if (equals(key, "name")) {
      pending_.locomotive.reset();
      if (!value.empty()) pending_.locomotive.setName(value.data());
    } else if (equals(key, "llindex")) {
      if (!readNumber(value, pending_.llindex)) invalid_ = true;
    } else if (equals(key, "crc")) {
      if (!readNumber(value, pending_.crc)) invalid_ = true;
    }
  }
}

size_t LokListeConsumer::getStoredCount() const {
  size_t count = 0;
  for (size_t i = 0; i < capacity_; ++i) {
    if (storage_[i].locomotive.isNameKnown()) ++count;
  }
  return count;
}

void LokListeConsumer::setStreamComplete() {
  if (invalid_) {
    setStreamAborted();
    return;
  }
  finishEntry();
  complete_ = true;
  if (streamEndCallback_ != nullptr) streamEndCallback_->streamComplete(this);
}

void LokListeConsumer::setStreamAborted() {
  clear();
  if (streamEndCallback_ != nullptr) streamEndCallback_->streamAborted(this);
}

}  // namespace RR32Can
