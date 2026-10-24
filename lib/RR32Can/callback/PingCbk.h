#ifndef __RR32CAN__CALLBACK__PINGCBK_H__
#define __RR32CAN__CALLBACK__PINGCBK_H__

#include "RR32Can/messages/CanFrame.h"
#include "RR32Can/messages/PingResponsePacket.h"

namespace RR32Can {
namespace callback {

/**
 * \brief Callback interface for ping requests and responses.
 */
class PingCbk {
 public:
  /**
   * \brief Handle a ping response.
   */
  virtual void HandlePingResponse(const PingResponsePacket& packet) = 0;

  /**
   * \brief Handle a ping request, so the application can send a response.
   */
  virtual void HandlePing(const CanFrame&) = 0;
};

}  // namespace callback
}  // namespace RR32Can

#endif  // __RR32CAN__CALLBACK__PINGCBK_H__
