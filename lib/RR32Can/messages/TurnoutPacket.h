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

  void initData() {
    BaseMessage::initData();
    data_.dlc = 6;
  }

  MachineTurnoutAddress getLocid() const { return MachineTurnoutAddress(data_.getLocid()); }
  void setLocid(const MachineTurnoutAddress& locid) { data_.setLocid(locid.value()); }

  bool getPower() const { return data_.data[5] != 0; }
  void setPower(bool power) { data_.data[5] = (power ? 1 : 0); }

  TurnoutDirection getDirection() const { return TurnoutDirectionFromIntegral(data_.data[4]); }
  void setDirection(TurnoutDirection direction) { data_.data[4] = TurnoutDirectionToIntegral(direction); }

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
};

} /* namespace RR32Can */

#endif  // SRC_RR32CAN_MESSAGES_TURNOUTPACKET_H_
