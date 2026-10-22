#ifndef SRC_RR32CAN_LOKLISTECONSUMERCALLBACK_H_
#define SRC_RR32CAN_LOKLISTECONSUMERCALLBACK_H_

#include "RR32Can/Locomotive.h"

namespace RR32Can {

  /**
   * This callback supplies storage for LocomotiveInfoShort to the LoklisteConsumer.
   * The intention is to have an ordered storage. Index then denotes the element in the storage at the given index.
   *
   * No actual requirements are placed on the storage, other than it must outlive the ongoing download by LoklisteConsumer.
   */
class LokListeConsumerCallback {
 public:
  virtual ~LokListeConsumerCallback() = default;
  virtual LocomotiveShortInfo* getLokInfoShort(uint32_t index) = 0;
};

}  // namespace RR32Can

#endif  // SRC_RR32CAN_LOKLISTECONSUMERCALLBACK_H_
