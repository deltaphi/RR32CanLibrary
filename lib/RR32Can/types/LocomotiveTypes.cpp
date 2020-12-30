#include "RR32Can/types/LocomotiveTypes.h"

namespace RR32Can {

EngineDirection switchDirection(EngineDirection direction) {
  if (direction == EngineDirection::FORWARD) {
    return EngineDirection::REVERSE;
  } else if (direction == EngineDirection::REVERSE) {
    return EngineDirection::FORWARD;
  } else {
    return EngineDirection::UNKNOWN;
  }
}

HumanLocomotiveAddress::HumanLocomotiveAddress(const MachineLocomotiveAddress& other)
    : LocomotiveAddressBase(other.value() + 1) {}

MachineLocomotiveAddress::MachineLocomotiveAddress(const HumanLocomotiveAddress& other)
    : LocomotiveAddressBase(other.value() - 1) {}

}  // namespace RR32Can
