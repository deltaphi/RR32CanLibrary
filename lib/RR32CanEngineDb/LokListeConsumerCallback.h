#ifndef SRC_RR32CAN_LOKLISTECONSUMERCALLBACK_H_
#define SRC_RR32CAN_LOKLISTECONSUMERCALLBACK_H_

#include "RR32Can/Locomotive.h"

namespace RR32Can {

class LokListeConsumerCallback {
 public:
  virtual ~LokListeConsumerCallback() = default;
  virtual LocomotiveShortInfo* getLokInfoShort(uint32_t index) = 0;
};

}  // namespace RR32Can

#endif  // SRC_RR32CAN_LOKLISTECONSUMERCALLBACK_H_
