#include "RR32Can/types/TurnoutTypes.h"

namespace RR32Can {

HumanTurnoutAddress::HumanTurnoutAddress(const MachineTurnoutAddress& other) : TurnoutAddressBase(other.value() + 1) {}

MachineTurnoutAddress::MachineTurnoutAddress(const HumanTurnoutAddress& other)
    : TurnoutAddressBase(other.value() - 1) {}

}  // namespace RR32Can
