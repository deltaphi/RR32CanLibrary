#ifndef __RR32CAN__TYPES__LOCOMOTIVETYPES_H__
#define __RR32CAN__TYPES__LOCOMOTIVETYPES_H__

#include "RR32Can/types/BaseTypes.h"

namespace RR32Can {

/// Engine Direction
enum class EngineDirection { UNCHANGED = 0, FORWARD = 1, REVERSE = 2, CHANGE_DIRECTION = 3, UNKNOWN = 4 };

EngineDirection switchDirection(EngineDirection);

/// Data type to carry engine velocities. Range: 1..1000
using Uid_t = uint32_t;
using Velocity_t = uint16_t;
using EngineAddress_t = uint32_t;
using FunctionBits_t = uint16_t;

/// Opaque Typedefs for Locomotive Addresses
class LocomotiveAddressBase {
 public:
  using value_type = LocId_t;
  constexpr value_type value() const { return addr_; }

  LocomotiveAddressBase() : addr_(0){};
  constexpr explicit LocomotiveAddressBase(value_type addr) : addr_(addr) {}

 protected:
  value_type addr_;
};

class MachineLocomotiveAddress;
MachineLocomotiveAddress getLocomotiveLocIdMask(RailProtocol proto);

/// 1-based engine address
class HumanLocomotiveAddress : public LocomotiveAddressBase {
 public:
  using LocomotiveAddressBase::LocomotiveAddressBase;
  HumanLocomotiveAddress(const MachineLocomotiveAddress& other);

  constexpr bool operator==(const HumanLocomotiveAddress& other) const { return value() == other.value(); };
  constexpr bool operator!=(const HumanLocomotiveAddress& other) const { return !operator==(other); };
};

/// 0-based engine address that can optionally have a protocol
class MachineLocomotiveAddress : public LocomotiveAddressBase {
 public:
  MachineLocomotiveAddress() = default;
  using LocomotiveAddressBase::LocomotiveAddressBase;
  MachineLocomotiveAddress(const HumanLocomotiveAddress& other);

  constexpr bool operator==(const MachineLocomotiveAddress& other) const { return value() == other.value(); };
  constexpr bool operator!=(const MachineLocomotiveAddress& other) const { return !operator==(other); };
  void operator|=(const MachineLocomotiveAddress& other) { addr_ = addr_ | other.value(); };
  constexpr bool operator<=(const MachineLocomotiveAddress& other) const { return value() <= other.value(); };

  /**
   * \brief Address without the protocol part.
   */
  MachineLocomotiveAddress getNumericAddress() const { return MachineLocomotiveAddress(value() & 0x03FF); }
  void setProtocol(RailProtocol protocol) {
    *this = getNumericAddress();
    *this |= getLocomotiveLocIdMask(protocol);
  }
};

constexpr MachineLocomotiveAddress operator|(const MachineLocomotiveAddress& left,
                                             const MachineLocomotiveAddress& right) {
  return MachineLocomotiveAddress{left.value() | right.value()};
}

}  // namespace RR32Can

#endif  // __RR32CAN__TYPES__LOCOMOTIVETYPES_H__
