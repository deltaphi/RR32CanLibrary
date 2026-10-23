#ifndef __RR32CAN__CALLBACK__SYSTEMCBK_H__
#define __RR32CAN__CALLBACK__SYSTEMCBK_H__

#include "RR32Can/messages/CanFrame.h"

namespace RR32Can {
namespace callback {

/*
 * \brief Class SystemCbk
 */
class SystemCbk {
 public:
  /**
   * \brief Set whether the system is on (true) or off (false).
   *
   * \param onOff Whether the system is on (true) or off (false).
   * \param response Whether the packet was a response (true) or a request (false).
   */
  virtual void setSystemState(bool onOff, bool response) = 0;

  /**
   * \brief Handle an eight-byte ping response, including its sender identifier.
   *
   * Payload bytes 4 and 5 contain the major and minor version. For MS2 devices,
   * byte 7 is in the range 0x30–0x3F. The default implementation ignores responses.
   */
  virtual void HandlePingResponse(const CanFrame&) {}
};

}  // namespace callback
}  // namespace RR32Can

#endif  // __RR32CAN__CALLBACK__SYSTEMCBK_H__
