#ifndef SRC_RR32CAN_MESSAGES_IDENTIFIER_H_
#define SRC_RR32CAN_MESSAGES_IDENTIFIER_H_

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

#include "RR32Can/Constants.h"
#include "RR32Can/util/utils.h"

namespace RR32Can {

/**
 * \brief Reflects the deserialized form of a CAN identifier as used by Marklin
 */
class Identifier {
 public:
  using value_type = uint32_t;

  constexpr static const value_type kHashBitMask = (0xFFFF);
  constexpr static const value_type kResponseBitMask = (1 << 16);
  constexpr static const value_type kCommandBitMask = (0xFF << 17);
  // constexpr static const value_type kPrioBitMask = (0x0F << 25);

  Identifier() : identifier_(0){};
  Identifier(value_type packetId) : identifier_(packetId){};
  Identifier(Command command, uint16_t hash) : identifier_(0) {
    setCommand(command);
    setHash(hash);
  }

  bool isResponse() const { return (identifier_ & kResponseBitMask) != 0; };
  void setResponse(bool response) {
    if (response) {
      identifier_ |= kResponseBitMask;
    } else {
      identifier_ &= ~kResponseBitMask;
    }
  }

  uint8_t getPriority() const { return (identifier_ >> 25) & 0x0F; }
  // void setPriority(uint8_t priority);

  Command getCommand() const { return static_cast<Command>((identifier_ >> 17) & 0xFF); };

  void setCommand(Command command) {
    // Shift commandByte to the correct place
    uint32_t commandByte = static_cast<CommandByte_t>(command);
    commandByte <<= 17;
    // Clear all command bits in identifier
    identifier_ &= ~kCommandBitMask;
    // Set commandByte bits
    identifier_ |= commandByte;
  }

  uint16_t getHash() const { return identifier_ & kHashBitMask; }

  bool operator==(const Identifier& other) const { return identifier_ == other.identifier_; }

  /**
   * \brief Turn a 32-bit CAN identifier into a Marklin Identifier
   */
  static Identifier GetIdentifier(value_type packetId) { return Identifier(packetId); }

  /**
   * \brief Turn a 4-byte buffer from LAN into a Marklin Identifier
   */
  static Identifier GetIdentifier(uint8_t buffer[4]);

  /**
   * \brief Determine the correct hash value given the address of this component
   */
  void computeAndSetHash(uint32_t deviceUuid) { setHash(computeSenderHash(deviceUuid)); }

  /**
   * \brief convert this Marklin Identifier into a 29bit CAN extended
   * identifier.
   */
  uint32_t makeIdentifier() const { return identifier_; }

  /**
   * \brief Print the identifier to serial in human-readable form.
   */
  void printAll() const;

  void setHash(uint16_t hash) {  // Clear hash bits
    identifier_ &= ~kHashBitMask;
    // Set new bits
    identifier_ |= hash;
  }

 private:
  value_type identifier_;
};

} /* namespace RR32Can */

#endif  // SRC_RR32CAN_MESSAGES_IDENTIFIER_H_
