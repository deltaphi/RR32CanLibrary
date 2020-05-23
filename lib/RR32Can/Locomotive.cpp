#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdio>
#endif

#include <RR32Can/Locomotive.h>

namespace RR32Can {

void LocomotiveShortInfo::print() const {
  printf("  EngineShortInfo: ");

  if (availability == AvailabilityStatus::EMPTY) {
    printf("Empty.\n");
  } else {
    printf("'%s'", name);
  }
}

void Locomotive::print() const {
  LocomotiveShortInfo::print();
  if (availability == AvailabilityStatus::FULL_DETAILS) {
    printf("UID: %#10x Proto: %s Addr: %i V: %i Dir: %i, F: %i\n", uid,
           protocol, address, velocity, static_cast<uint8_t>(direction),
           functionBits);
  }
}

}  // namespace RR32Can
