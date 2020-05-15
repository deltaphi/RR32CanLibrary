#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#include <cstdio>
#endif

#include "RR32Can/messages/Identifier.h"

#include "RR32Can/util/utils.h"

namespace RR32Can {

Identifier Identifier::GetIdentifier(uint8_t buffer[4]) {
  uint32_t intermediateBits =
      (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[1];
  return GetIdentifier(intermediateBits);
}

Identifier Identifier::GetIdentifier(unsigned long packetId) {
  Identifier id;

  id.hash = static_cast<uint16_t>(packetId & 0xFFFF);
  packetId >>= 16;  // Shift away hash

  id.response = (packetId & 0x01) != 0;
  packetId >>= 1;  // Shift away response bit

  id.command = static_cast<uint8_t>(packetId & 0xFF);
  packetId >>= 8;  // Shift away command bit

  id.prio = static_cast<uint8_t>(packetId & 0x0F);

  return id;
}

unsigned long Identifier::makeIdentifier() const {
  unsigned long packetId;

  packetId = prio << (16 + 1 + 8);
  packetId |= command << (16 + 1);
  packetId |= response << (16);
  packetId |= hash;

  return packetId;
}

void Identifier::printAll() const {
  printf("Prio: %#04X, Command: %#04X, Response: %#04X, Hash: %#06X", prio, command, response, hash);
}

} /* namespace RR32Can */
