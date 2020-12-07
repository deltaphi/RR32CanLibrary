#include "RR32Can/messages/TurnoutPacket.h"

#include "RR32Can/StlAdapter.h"

namespace RR32Can {

TurnoutPacket TurnoutPacket::FromCanPacket(const Data& data) {
  TurnoutPacket turnoutPacket;

  using LogcId_t = TurnoutAddressBase::value_type;
  turnoutPacket.locid = static_cast<LogcId_t>(data.data[3]) | (static_cast<LogcId_t>(data.data[2]) << 8) |
                        (static_cast<LogcId_t>(data.data[1]) << 16) | (static_cast<LogcId_t>(data.data[0]) << 24);
  turnoutPacket.position = TurnoutDirectionFromIntegral(data.data[4]);
  turnoutPacket.power = data.data[5];

  return turnoutPacket;
}

void TurnoutPacket::serialize(Data& data) const {
  data.dlc = 6;
  data.data[0] = (locid.value() >> 24) & 0xFF;
  data.data[1] = (locid.value() >> 16) & 0xFF;
  data.data[2] = (locid.value() >> 8) & 0xFF;
  data.data[3] = locid.value() & 0xFF;
  data.data[4] = TurnoutDirectionToIntegral<uint8_t>(position);
  data.data[5] = power;
}

HumanTurnoutAddress TurnoutPacket::AddressAsHumanValue() const { return HumanTurnoutAddress(locid); }

void TurnoutPacket::printAll() const {
  printf("Turnout: %#10x, Human Value: %d, Position: %d ", locid, AddressAsHumanValue(), position);

  switch (this->position) {
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

  printf(" Power: %d ", power);
  switch (this->power) {
    case 0:
      printf("(off, button release)");
      break;
    case 1:
      printf("(on, button press)");
      break;
  }
}

bool TurnoutPacket::operator==(const TurnoutPacket& other) const {
  return (this->locid == other.locid) && (this->position == other.position) && (this->power == other.power);
}

} /* namespace RR32Can */
