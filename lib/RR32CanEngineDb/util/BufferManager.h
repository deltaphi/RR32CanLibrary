#ifndef SRC_RR32CAN_UTIL_BUFFERMANAGER_H_
#define SRC_RR32CAN_UTIL_BUFFERMANAGER_H_

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#include <cstdlib>
#include <cstring>
#endif

namespace RR32Can {

class BufferManager {
 public:
  using value_type = char;
  using size_type = uint8_t;

  constexpr static const size_type npos = -1;

  constexpr BufferManager() : BufferManager(nullptr, 0, 0) {}

  constexpr BufferManager(value_type* buffer, size_type bufferCapacity) : BufferManager(buffer, bufferCapacity, 0) {}

  constexpr BufferManager(value_type* buffer, size_type bufferCapacity, size_type currentContentLength)
      : buffer_(buffer), bufferCapacity_(bufferCapacity), currentContentLength_(currentContentLength) {}

  BufferManager(const BufferManager&) = delete;
  BufferManager& operator=(const BufferManager&) = delete;
  BufferManager(BufferManager&&) = default;
  BufferManager& operator=(BufferManager&&) = default;

  ~BufferManager() = default;

  size_type limitIndex(size_type index) const {
    return index >= currentContentLength_ ? currentContentLength_ - 1 : index;
  }

  value_type operator[](size_type index) const { return buffer_[limitIndex(index)]; }

  bool strncmp(const char* other) { return 0 == (::strncmp(buffer_, other, currentContentLength_)); }

  /*
   * \brief Get a reference to the character at the index.
   *
   * If index is outside of the currently valid range of elements, it is limited
   * to the last element.
   */
  value_type& at(size_type index) { return buffer_[limitIndex(index)]; }

  size_type findFirstOf(value_type character, size_type offset = 0) const;
  size_type findFirstOf(const value_type* chars, size_type offset = 0) const;

  void erase();

  /**
   * \brief Remove the first num_elements elements and move the remainder of the
   * buffer to the front
   */
  void pop_front(size_type num_elements);

  constexpr size_type capacity() const { return bufferCapacity_; }

  constexpr size_type capacity_remaining() const { return bufferCapacity_ - currentContentLength_; }

  constexpr bool full() const { return capacity_remaining() == 0; }

  constexpr bool empty() const { return length() == 0; }

  constexpr size_type length() const { return currentContentLength_; }

  constexpr value_type* data() const { return buffer_; }

  uint8_t asUint8() const { return static_cast<uint8_t>(strtol(buffer_, nullptr, 10)); }

  /*
   * \brief Read as many bytes as possible from the otherBuffer.
   *
   * There may be unread bytes left in the otherBuffer.
   * Does not modify otherBuffer.
   *
   * \return The number of bytes actually read.
   */
  size_type push_back(const BufferManager& otherBuffer);

  /*
   * \brief Add a single character to the end of the buffer.
   *
   * If the buffer is full, it remains unmodified.
   *
   * \return The number of characters added to the buffer.
   */
  size_type push_back(const char c) {
    if (currentContentLength_ < bufferCapacity_) {
      buffer_[currentContentLength_] = c;
      ++currentContentLength_;
      return 1;
    } else {
      return 0;
    }
  }

  /*
   * \brief Set the buffer to a given string value.
   *
   * Note that the BufferManager will refuse to set the last byte using this
   * method - it will always be a '\0'
   */
  void set(const char* str);

  size_type move_back(BufferManager& otherBuffer);

  /**
   * \brief Creates a view-buffer on a section of this buffer.
   *
   * Do not use the returned buffer for modificatoins of the underlying memory.
   * Results will definitely be surprising.
   */
  BufferManager subBufferManager(size_type start_offset) const;
  BufferManager subBufferManager(size_type start_offset, size_type end_offset) const;

 private:
  value_type* buffer_;
  size_type bufferCapacity_;
  size_type currentContentLength_;
};

} /* namespace RR32Can */

#endif  // SRC_RR32CAN_UTIL_BUFFERMANAGER_H_
