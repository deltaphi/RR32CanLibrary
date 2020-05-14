#ifndef __RR32CAN__STATIONTXCBK_H__
#define __RR32CAN__STATIONTXCBK_H__

#include "RR32Can/Types.h"
#include "RR32Can/messages/Data.h"
#include "RR32Can/messages/Identifier.h"

namespace RR32Can {

/*
 * \brief Class StationTxCbk
 */
class StationTxCbk {
 public:
  /**
   * \brief Send an arbitrary packet via CAN
   */
  virtual void SendPacket(const RR32Can::Identifier& id, const RR32Can::Data& data) = 0;

 private:
};

}  // namespace RR32Can

#endif  // __RR32CAN__STATIONTXCBK_H__
