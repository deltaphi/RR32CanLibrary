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
   * \brief Handle a ping response.
   */
  virtual void HandlePingResponse(LocId_t deviceId, uint8_t swVersionMajor, uint8_t swVersionMinor,
                                  uint16_t deviceInformation) = 0;

  /**
   * \brief Handle a ping request, so the application can send a response.
   */
  virtual void HandlePing(const CanFrame&) = 0;
};

}  // namespace callback
}  // namespace RR32Can

#endif  // __RR32CAN__CALLBACK__SYSTEMCBK_H__
