#ifndef SRC_RR32CAN_UTIL_UTILS_H_
#define SRC_RR32CAN_UTIL_UTILS_H_

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

namespace RR32Can {

uint16_t computeSenderHash(uint32_t uid);

}  // namespace RR32Can

#endif  // SRC_RR32CAN_UTIL_UTILS_H_
