#ifndef __RR32CAN__TYPES__TURNOUTTYPES_H__
#define __RR32CAN__TYPES__TURNOUTTYPES_H__

#include "RR32Can/types/BaseTypes.h"

namespace RR32Can {

class MachineTurnoutAddress;
MachineTurnoutAddress getAccessoryLocIdMask(RailProtocol proto);

/// Accessory direction.
using TurnoutDirection_UnderlyingType = uint8_t;
enum class TurnoutDirection : TurnoutDirection_UnderlyingType { RED = 0, GREEN = 1, YELLOW = 2, WHITE = 3 };

constexpr TurnoutDirection_UnderlyingType TurnoutDirectionToIntegral(TurnoutDirection dir) {
  return static_cast<TurnoutDirection_UnderlyingType>(dir);
}

constexpr TurnoutDirection TurnoutDirectionFromIntegral(TurnoutDirection_UnderlyingType dir) {
  return static_cast<TurnoutDirection>(dir);
}

/**
 * \brief Base class for all turnout addresses.
 *
 * Used to implement a C++ strong type for Turnout Addresses.
 *
 * Human addresses are 1-based, whereas machine addresses are 0-based.
 */
class TurnoutAddressBase {
 public:
  using value_type = LocId_t;
  constexpr value_type value() const { return addr_; }

  TurnoutAddressBase() : addr_(0){};
  constexpr explicit TurnoutAddressBase(value_type addr) : addr_(addr) {}

 protected:
  value_type addr_;
};

/**
 * \brief The Human-readable version of the Turnout Address.
 */
class HumanTurnoutAddress : public TurnoutAddressBase {
 public:
  using TurnoutAddressBase::TurnoutAddressBase;
  HumanTurnoutAddress(const MachineTurnoutAddress& other);

  constexpr bool operator==(const HumanTurnoutAddress& other) const { return value() == other.value(); };
  constexpr bool operator!=(const HumanTurnoutAddress& other) const { return !operator==(other); };
};

/**
 * \brief The Machine-readable version of the Turnout Address.
 */
class MachineTurnoutAddress : public TurnoutAddressBase {
 public:
  MachineTurnoutAddress() = default;
  using TurnoutAddressBase::TurnoutAddressBase;
  MachineTurnoutAddress(const HumanTurnoutAddress& other);

  constexpr bool operator==(const MachineTurnoutAddress& other) const { return value() == other.value(); };
  constexpr bool operator!=(const MachineTurnoutAddress& other) const { return !operator==(other); };
  constexpr void operator|=(const MachineTurnoutAddress& other) { addr_ = addr_ | other.value(); };

  constexpr bool operator<=(const MachineTurnoutAddress& other) const { return value() <= other.value(); };

  /**
   * \brief Address without the protocol part.
   */
  constexpr MachineTurnoutAddress getNumericAddress() const { return MachineTurnoutAddress(value() & 0x03FF); }
  void setProtocol(RailProtocol protocol) {
    *this = getNumericAddress();
    *this |= getAccessoryLocIdMask(protocol);
  }
};

constexpr MachineTurnoutAddress operator|(const MachineTurnoutAddress& left, const MachineTurnoutAddress& right) {
  return MachineTurnoutAddress{left.value() | right.value()};
}

}  // namespace RR32Can

#endif  // __RR32CAN__TYPES__TURNOUTTYPES_H__
