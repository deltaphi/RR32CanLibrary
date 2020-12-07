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
  /// Turnout-address (0-based)
  MachineTurnoutAddress locid = 0;

  /// Whether to set the turnout to straight or branching
  TurnoutDirection position = TurnoutDirection::RED;

  /// Whether to switch the actuator on or off
  uint8_t power = 0;

  /**
   * \brief Parse a CAN payload into a turnout packet.
   *
   * This does not check the type of the contents of Data.
   */
  static TurnoutPacket FromCanPacket(const Data& data);

  /**
   * \brief Serialize the fields of this TurnoutPacket into a CAN payload.
   */
  void serialize(Data& data) const;

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
