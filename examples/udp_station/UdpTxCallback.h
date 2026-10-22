#ifndef UDP_STATION_UDP_TX_CALLBACK_H
#define UDP_STATION_UDP_TX_CALLBACK_H

#include "RR32Can/callback/TxCbk.h"

#include "UdpConnection.h"

namespace udp_station {

class UdpTxCallback : public RR32Can::callback::TxCbk {
 public:
  explicit UdpTxCallback(UdpConnection& connection);

  void SendPacket(const RR32Can::CanFrame& frame) override;

 private:
  UdpConnection& connection_;
};

}  // namespace udp_station

#endif  // UDP_STATION_UDP_TX_CALLBACK_H
