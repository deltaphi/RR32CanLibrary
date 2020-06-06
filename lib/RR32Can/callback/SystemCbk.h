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
   * \brief Set whether the system is on (true) or off (false)
   */
  virtual void setSystemState(bool onOff) = 0;
};

}  // namespace callback
}  // namespace RR32Can

#endif  // __RR32CAN__CALLBACK__SYSTEMCBK_H__
