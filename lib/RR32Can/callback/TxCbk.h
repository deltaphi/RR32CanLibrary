#ifndef __RR32CAN__CALLBACK__TXCBK_H__
#define __RR32CAN__CALLBACK__TXCBK_H__

#include "RR32Can/messages/Data.h"
#include "RR32Can/messages/Identifier.h"

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
  virtual void SendPacket(const RR32Can::Identifier& id, const RR32Can::Data& data) = 0;
};

}  // namespace callback
}  // namespace RR32Can

#endif  // __RR32CAN__CALLBACK__TXCBK_H__
