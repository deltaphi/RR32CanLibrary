#ifndef SRC_RR32CAN_MESSAGES_DATA_H_
#define SRC_RR32CAN_MESSAGES_DATA_H_

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#include <cstring>
#endif

#include "RR32Can/Constants.h"

namespace RR32Can {

/**
 * \brief Class to represent the payload of a Marklin CAN packet.
 *
 * This class represents the untyped payload and can be sent or received from
 * CAN.
 */
struct Data {
  constexpr static const uint8_t kDataBufferLength = CanDataMaxLength + 1;

  /// Data Length C... . actual value ist 4 bit only.
  uint8_t dlc = 0;
  /// Actual data. An extra byte is prepared to allow for the use of snprintf.
  uint8_t data[kDataBufferLength] = {0, 0, 0, 0, 0, 0, 0, 0};

  bool operator==(const Data& other) const {
    if (dlc != other.dlc) {
      return false;
    } else {
      return 0 == memcmp(data, other.data, dlc);
    }
  }

  /**
   * \brief Print the DLC and Data field as Hex values.
   */
  void printAsHex() const;

  /**
   * \brief Print the Payload as characters.
   */
  void printAsText() const;

  const char* dataAsString() const {
    const uint8_t* ptr = data;
    return reinterpret_cast<const char*>(ptr);
  }

  char* dataAsString() {
    uint8_t* ptr = data;
    return reinterpret_cast<char*>(ptr);
  }

  void reset() {
    dlc = 0;
    memset(data, 0, kDataBufferLength);
  }

  constexpr LocId_t getLocid() const {
    LocId_t locid = static_cast<LocId_t>(data[3]) | (static_cast<LocId_t>(data[2]) << 8) |
                    (static_cast<LocId_t>(data[1]) << 16) | (static_cast<LocId_t>(data[0]) << 24);
    return locid;
  }

  constexpr void setLocid(LocId_t locid) {
    data[0] = (locid >> 24);
    data[1] = (locid >> 16);
    data[2] = (locid >> 8);
    data[3] = locid;
  }
};

} /* namespace RR32Can */

#endif  // SRC_RR32CAN_MESSAGES_DATA_H_