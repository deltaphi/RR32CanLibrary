#ifndef __RR32CAN__CALLBACK__TXCBK_H__
#define __RR32CAN__CALLBACK__TXCBK_H__

#include "RR32Can/messages/CanFrame.h"

namespace RR32Can {
namespace callback {

/*
 * \brief Class TxCbk
 */
class TxCbk {
 public:
  /**
   * \brief Send an arbitrary packet via CAN
   */
  virtual void SendPacket(const RR32Can::CanFrame& canFrame) = 0;
};

}  // namespace callback
}  // namespace RR32Can

#endif  // __RR32CAN__CALLBACK__TXCBK_H__
