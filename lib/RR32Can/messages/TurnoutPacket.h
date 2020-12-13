#ifndef SRC_RR32CAN_MESSAGES_TURNOUTPACKET_H_
#define SRC_RR32CAN_MESSAGES_TURNOUTPACKET_H_

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

#include "RR32Can/messages/Data.h"

namespace RR32Can {

/**
 * \brief Resembles a turnout command.
 */
class TurnoutPacket {
 public:
  TurnoutPacket(RR32Can::Data& data) : data_(data){};

  void initData() {
    data_.dlc = 6;
    memset(data_.data, 0, sizeof(&(data_.data)));
  }

  MachineTurnoutAddress getLocid() const {
    using LogcId_t = TurnoutAddressBase::value_type;
    MachineTurnoutAddress locid = static_cast<LogcId_t>(data_.data[3]) | (static_cast<LogcId_t>(data_.data[2]) << 8) |
                                  (static_cast<LogcId_t>(data_.data[1]) << 16) |
                                  (static_cast<LogcId_t>(data_.data[0]) << 24);
    return locid;
  }
  void setLocid(const MachineTurnoutAddress& locid) {
    data_.data[0] = (locid.value() >> 24) & 0xFF;
    data_.data[1] = (locid.value() >> 16) & 0xFF;
    data_.data[2] = (locid.value() >> 8) & 0xFF;
    data_.data[3] = locid.value() & 0xFF;
  }

  bool getPower() const { return data_.data[5] != 0; }
  void setPower(bool power) { data_.data[5] = (power ? 1 : 0); }

  TurnoutDirection getDirection() const { return TurnoutDirectionFromIntegral(data_.data[4]); }
  void setDirection(TurnoutDirection direction) { data_.data[4] = TurnoutDirectionToIntegral<uint8_t>(direction); }

  /**
   * \brief Obtain the human-readable turnout address (1-based)
   */
  HumanTurnoutAddress AddressAsHumanValue() const;

  /**
   * \brief Print a human-readable version of this Turnout Packet.
   */
  void printAll() const;

  bool operator==(const TurnoutPacket& other) const;

  /**
   * \brief Return the Rail Protocol for the turnout.
   *
   * \return MM1, DCC, SX1 or UNKOWN.
   */
  RailProtocol getRailProtocol() const {
    MachineTurnoutAddress locid = getLocid();
    if (kSX1AccessoryAddrStart <= locid && locid <= kSX1AccessoryAddrStop) {
      return RailProtocol::SX1;
    } else if (kMMAccessoryAddrStart <= locid && locid <= kMMAccessoryAddrStop) {
      return RailProtocol::MM1;
    } else if (kDCCAccessoryAddrStart <= locid && locid <= kDCCccessoryAddrStop) {
      return RailProtocol::DCC;
    } else {
      return RailProtocol::UNKNOWN;
    }
  }

 private:
  RR32Can::Data& data_;
};

} /* namespace RR32Can */

#endif  // SRC_RR32CAN_MESSAGES_TURNOUTPACKET_H_
