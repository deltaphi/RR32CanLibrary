#ifndef SRC_RR32CAN_MESSAGES_PINGRESPONSEPACKET_H_
#define SRC_RR32CAN_MESSAGES_PINGRESPONSEPACKET_H_

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

#include "RR32Can/messages/BaseMessage.h"

namespace RR32Can {

/**
 * \brief Represents the payload of a ping response.
 */
class PingResponsePacket : public BaseMessage {
 public:
  using BaseMessage::BaseMessage;

  constexpr void initData() {
    BaseMessage::initData();
    data_.dlc = 8;
  }

  constexpr LocId_t getLocid() const { return data_.getLocid(); }
  constexpr void setLocid(LocId_t locid) { data_.setLocid(locid); }

  constexpr uint8_t getSoftwareVersionMajor() const { return data_.data[4]; }
  constexpr void setSoftwareVersionMajor(uint8_t version) { data_.data[4] = version; }

  constexpr uint8_t getSoftwareVersionMinor() const { return data_.data[5]; }
  constexpr void setSoftwareVersionMinor(uint8_t version) { data_.data[5] = version; }

  constexpr uint16_t getDeviceInformation() const {
    return (static_cast<uint16_t>(data_.data[6]) << 8) | data_.data[7];
  }
  constexpr void setDeviceInformation(uint16_t information) {
    data_.data[6] = static_cast<uint8_t>(information >> 8);
    data_.data[7] = static_cast<uint8_t>(information);
  }
};

}  // namespace RR32Can

#endif  // SRC_RR32CAN_MESSAGES_PINGRESPONSEPACKET_H_
