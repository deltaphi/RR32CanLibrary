#include "RR32Can/messages/TurnoutPacket.h"

#include "RR32Can/StlAdapter.h"

namespace RR32Can {

void TurnoutPacket::printAll() const {
  RR32Can::TurnoutDirection position = getDirection();
  printf("Turnout: %#10x, Human Value: %d, Position: %d ", getLocid().value(), AddressAsHumanValue().value(),
         TurnoutDirectionToIntegral(position));

  switch (position) {
    case TurnoutDirection::RED:
      printf("(off, round, red)");
      break;
    case TurnoutDirection::GREEN:
      printf("(on, straight, green)");
      break;
    case TurnoutDirection::YELLOW:
      printf("(yellow, left)");
      break;
    case TurnoutDirection::WHITE:
      printf("(white)");
      break;
  }

  bool power = getPower();
  printf(" Power: %d ", power);
  if (!power) {
    printf("(off, button release)");
  } else {
    printf("(on, button press)");
  }
}

bool TurnoutPacket::operator==(const TurnoutPacket& other) const {
  return (getLocid() == other.getLocid()) && (getDirection() == other.getDirection()) &&
         (this->getPower() == other.getPower());
}

} /* namespace RR32Can */
