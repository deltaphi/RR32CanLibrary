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
   *
   * \param packet The packet that was received.
   * \param request Whether the packet was a request (false) or a response (true).
   */
  virtual void OnAccessoryPacket(TurnoutPacket& packet, bool response) = 0;
};

}  // namespace callback
}  // namespace RR32Can

#endif  // __RR32CAN__CALLBACK__ACCESSORYCBK_H__
