#include "RR32Can/types/LocomotiveTypes.h"

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

}  // namespace RR32Can
