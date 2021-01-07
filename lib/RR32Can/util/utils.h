#ifndef SRC_RR32CAN_UTIL_UTILS_H_
#define SRC_RR32CAN_UTIL_UTILS_H_

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

namespace RR32Can {

constexpr uint16_t computeSenderHash(uint32_t uid) {
  uid = ((uid >> 3) & 0xFFFFFF00) | (uid & 0x7F);
  const uint16_t lowBytes = uid;
  const uint16_t highBytes = (uid >> 16);
  return lowBytes ^ highBytes;
}

}  // namespace RR32Can

#endif  // SRC_RR32CAN_UTIL_UTILS_H_
