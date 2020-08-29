#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstring>
#endif

#include "RR32CanEngineDb/util/BufferManager.h"

namespace RR32Can {

const BufferManager::size_type BufferManager::npos;

BufferManager::size_type BufferManager::findFirstOf(value_type character, size_type offset) const {
  if (offset >= currentBufferLength_) {
    return npos;
  }
  for (size_type i = offset; i < currentBufferLength_; ++i) {
    if (buffer_[i] == character) {
      return i;
    }
  }
  return npos;
}

BufferManager::size_type BufferManager::findFirstOf(const value_type* chars, size_type offset) const {
  if (offset >= currentBufferLength_) {
    return npos;
  }

  size_type numChars = static_cast<size_type>(strlen(chars));

  for (size_type i = offset; i < currentBufferLength_; ++i) {
    for (size_type j = 0; j < numChars; ++j) {
      if (buffer_[i] == chars[j]) {
        return i;
      }
    }
  }
  return npos;
}

void BufferManager::erase() {
  memset(buffer_, '\0', maxBufferLength_);
  currentBufferLength_ = 0;
}

/**
 * \brief Remove the first num_elements elements and move the remainder of the
 * buffer to the front
 */
void BufferManager::pop_front(size_type num_elements) {
  if (num_elements >= currentBufferLength_) {
    erase();
  } else {
    currentBufferLength_ -= num_elements;
    memmove(buffer_, buffer_ + num_elements, currentBufferLength_);
    memset(buffer_ + currentBufferLength_, '\0', capacity_remaining());
  }
}

BufferManager::size_type BufferManager::push_back(const BufferManager& otherBuffer) {
  size_type bytesToCopy =
      this->capacity_remaining() < otherBuffer.length() ? this->capacity_remaining() : otherBuffer.length();
  memcpy(buffer_ + currentBufferLength_, otherBuffer.data(), bytesToCopy);
  currentBufferLength_ += bytesToCopy;
  return bytesToCopy;
}

void BufferManager::set(const char* str) {
  erase();
  strncpy(buffer_, str, maxBufferLength_ - 1);  // ensure a 0-byte
  currentBufferLength_ = static_cast<size_type>(strlen(buffer_));
}

BufferManager::size_type BufferManager::move_back(BufferManager& otherBuffer) {
  auto movedBytes = this->push_back(otherBuffer);
  otherBuffer.pop_front(movedBytes);
  return movedBytes;
}

BufferManager BufferManager::subBufferManager(size_type start_offset) const {
  return subBufferManager(start_offset, length());
}

BufferManager BufferManager::subBufferManager(size_type start_offset, size_type end_offset) const {
  if (start_offset > maxBufferLength_) {
    start_offset = maxBufferLength_;
  }

  if (end_offset > maxBufferLength_) {
    end_offset = maxBufferLength_;
  }

  if (end_offset < start_offset) {
    end_offset = start_offset;
  }

  BufferManager mgr(buffer_ + start_offset, maxBufferLength_ - start_offset, end_offset - start_offset);

  return mgr;
}

} /* namespace RR32Can */
