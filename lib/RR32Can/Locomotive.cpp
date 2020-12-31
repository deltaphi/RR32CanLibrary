#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdio>
#endif

#include <RR32Can/Locomotive.h>

namespace RR32Can {

void LocomotiveShortInfo::print() const {
  printf("  EngineShortInfo: ");

  if (availability_ == AvailabilityStatus::EMPTY) {
    printf("Empty.\n");
  } else {
    printf("'%s'", name_);
  }
}

void Locomotive::print() const {
  LocomotiveShortInfo::print();
  if (availability_ == AvailabilityStatus::FULL_DETAILS) {
    printf("UID: %#10x Proto: %s Addr: %i V: %i Dir: %i, F: %i\n", uid_, protocol_, address_.value(), velocity_,
           static_cast<uint8_t>(direction_), functionBits_);
  }
}

}  // namespace RR32Can
