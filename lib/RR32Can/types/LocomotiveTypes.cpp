#include "RR32Can/types/LocomotiveTypes.h"
#include "RR32Can/Constants.h"

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

MachineLocomotiveAddress getLocomotiveLocIdMask(const RailProtocol proto) {
  switch (proto) {
    case RailProtocol::MM1:
    case RailProtocol::MM2:
      return kMMEngineFunctionAddrStart;
      break;
    case RailProtocol::MFX:
      return kMFXAddrStart;
      break;
    case RailProtocol::SX1:
    case RailProtocol::SX2:
      return kSX1AddrStart;
      break;
    case RailProtocol::DCC:
      return kDCCAddrStart;
      break;
    default:
      return MachineLocomotiveAddress(0xFF00);  // Guard Value that shows up as an error.
  }
}

}  // namespace RR32Can
