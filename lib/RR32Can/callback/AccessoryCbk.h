#ifndef __RR32CAN__CALLBACK__ACCESSORYCBK_H__
#define __RR32CAN__CALLBACK__ACCESSORYCBK_H__

#include "RR32Can/messages/TurnoutPacket.h"

namespace RR32Can {
namespace callback {

/*
 * \brief Class AccessoryCbk
 */
class AccessoryCbk {
 public:
  /**
   * \brief Called when an accessory packet was received.
   */
  virtual void OnAccessoryPacket(TurnoutPacket& packet) = 0;
};

}  // namespace callback
}  // namespace RR32Can

#endif  // __RR32CAN__CALLBACK__ACCESSORYCBK_H__
