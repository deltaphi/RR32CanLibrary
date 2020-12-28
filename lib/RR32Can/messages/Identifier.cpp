#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#include <cstdio>
#endif

#include "RR32Can/StlAdapter.h"

#include "RR32Can/messages/Identifier.h"

#include "RR32Can/util/utils.h"

namespace RR32Can {

Identifier Identifier::GetIdentifier(uint8_t buffer[4]) {
  uint32_t intermediateBits = (static_cast<uint32_t>(buffer[0]) << 24) | (static_cast<uint32_t>(buffer[1]) << 16) |
                              (static_cast<uint32_t>(buffer[2]) << 8) | buffer[3];
  return GetIdentifier(intermediateBits);
}

void Identifier::printAll() const {
  printf("Prio: %#04X, Command: %#04X, Response: %#04X, Hash: %#06X", getPriority(),
         static_cast<CommandByte_t>(getCommand()), isResponse(), getHash());
}

} /* namespace RR32Can */
