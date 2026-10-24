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
   * \brief Handle an eight-byte ping response.
   *
   * The payload contains the sender Loc-ID (bytes 0-3), software version
   * major/minor (bytes 4/5), and device information (bytes 6-7). The
   * multi-byte values are encoded big-endian.
   */
  virtual void HandlePingResponse(LocId_t locid, uint8_t swVersionMajor, uint8_t swVersionMinor,
                                  uint16_t deviceInformation) = 0;

  /**
   * \brief Handle an empty ping request so the application can send a response.
   *
   * The default implementation ignores requests.
   */
  virtual void HandlePing(const CanFrame&) = 0;
};

}  // namespace callback
}  // namespace RR32Can

#endif  // __RR32CAN__CALLBACK__SYSTEMCBK_H__
