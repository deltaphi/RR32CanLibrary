#include "RR32Can/types/TurnoutTypes.h"
#include "RR32Can/Constants.h"

namespace RR32Can {

HumanTurnoutAddress::HumanTurnoutAddress(const MachineTurnoutAddress& other) : TurnoutAddressBase(other.value() + 1) {}

MachineTurnoutAddress::MachineTurnoutAddress(const HumanTurnoutAddress& other)
    : TurnoutAddressBase(other.value() - 1) {}

MachineTurnoutAddress getAccessoryLocIdMask(const RailProtocol proto) {
  switch (proto) {
    case RailProtocol::MM1:
    case RailProtocol::MM2:
    case RailProtocol::MFX:
      return kMMAccessoryAddrStart;
      break;
    case RailProtocol::SX1:
    case RailProtocol::SX2:
      return kSX1AccessoryAddrStart;
      break;
    case RailProtocol::DCC:
      return kDCCAccessoryAddrStart;
      break;
    default:
      return MachineTurnoutAddress(0xFF00);  // Guard Value that shows up as an error.
  }
}

}  // namespace RR32Can
