#ifndef TCP_STATION_TCP_TX_CALLBACK_H
#define TCP_STATION_TCP_TX_CALLBACK_H

#include "RR32Can/callback/TxCbk.h"

#include "TcpConnection.h"

namespace tcp_station {

class TcpTxCallback : public RR32Can::callback::TxCbk {
 public:
  explicit TcpTxCallback(TcpConnection& connection);

  void SendPacket(const RR32Can::CanFrame& frame) override;

 private:
  TcpConnection& connection_;
};

}  // namespace tcp_station

#endif  // TCP_STATION_TCP_TX_CALLBACK_H
