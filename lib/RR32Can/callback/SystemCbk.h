#ifndef __RR32CAN__CALLBACK__SYSTEMCBK_H__
#define __RR32CAN__CALLBACK__SYSTEMCBK_H__

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
   * \param request Whether the packet was a response (true) or a request (false).
   */
  virtual void setSystemState(bool onOff, bool response) = 0;
};

}  // namespace callback
}  // namespace RR32Can

#endif  // __RR32CAN__CALLBACK__SYSTEMCBK_H__
