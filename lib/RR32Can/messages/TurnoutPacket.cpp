#include "RR32Can/messages/TurnoutPacket.h"

#include <cstdio>

namespace RR32Can {

TurnoutPacket TurnoutPacket::FromCanPacket(const Data& data) {
  TurnoutPacket turnoutPacket;

  turnoutPacket.locid = data.data[3] | (data.data[2] << 8) |
                        (data.data[1] << 16) | (data.data[0] << 24);
  turnoutPacket.position = data.data[4];
  turnoutPacket.power = data.data[5];

  return turnoutPacket;
}

void TurnoutPacket::serialize(Data& data) const {
  data.dlc = 6;
  data.data[0] = (locid >> 24) & 0xFF;
  data.data[1] = (locid >> 16) & 0xFF;
  data.data[2] = (locid >> 8) & 0xFF;
  data.data[3] = locid & 0xFF;
  data.data[4] = position;
  data.data[5] = power;
}

uint32_t TurnoutPacket::PositionAsHumanValue() const {
  decltype(locid) locid_human = (locid & 0x0FFF) + 1;
  return locid_human;
}

void TurnoutPacket::printAll() const {
  printf("Turnout: %#10x, Human Value: %d, Position: %d ", locid, PositionAsHumanValue(), position);
  
  switch (this->position) {
    case 0:
      printf("(off, round, red)");
      break;
    case 1:
      printf("(on, straight, green)");
      break;
    case 2:
      printf("(yellow, left)");
      break;
    case 3:
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

} /* namespace RR32Can */
