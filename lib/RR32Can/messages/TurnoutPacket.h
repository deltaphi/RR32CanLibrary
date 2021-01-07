#ifndef SRC_RR32CAN_MESSAGES_TURNOUTPACKET_H_
#define SRC_RR32CAN_MESSAGES_TURNOUTPACKET_H_

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

#include "RR32Can/messages/BaseMessage.h"

namespace RR32Can {

/**
 * \brief Resembles a turnout command.
 */
class TurnoutPacket : public BaseMessage {
 public:
  using BaseMessage::BaseMessage;

  constexpr void initData() {
    BaseMessage::initData();
    data_.dlc = 6;
  }

  constexpr MachineTurnoutAddress getLocid() const { return MachineTurnoutAddress(data_.getLocid()); }
  constexpr void setLocid(const MachineTurnoutAddress& locid) { data_.setLocid(locid.value()); }

  constexpr bool getPower() const { return data_.data[5] != 0; }
  constexpr void setPower(bool power) { data_.data[5] = (power ? 1 : 0); }

  constexpr TurnoutDirection getDirection() const { return TurnoutDirectionFromIntegral(data_.data[4]); }
  constexpr void setDirection(TurnoutDirection direction) { data_.data[4] = TurnoutDirectionToIntegral(direction); }

  /**
   * \brief Obtain the human-readable turnout address (1-based)
   */
  HumanTurnoutAddress AddressAsHumanValue() const { return HumanTurnoutAddress(getLocid()); }

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
  constexpr RailProtocol getRailProtocol() const {
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
};

} /* namespace RR32Can */

#endif  // SRC_RR32CAN_MESSAGES_TURNOUTPACKET_H_
