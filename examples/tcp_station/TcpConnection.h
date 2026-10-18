#ifndef TCP_STATION_TCP_CONNECTION_H
#define TCP_STATION_TCP_CONNECTION_H

#include "TcpProtocol.h"

namespace tcp_station {

class TcpConnection {
 public:
  TcpConnection(const char* host, const char* port);
  ~TcpConnection();

  TcpConnection(const TcpConnection&) = delete;
  TcpConnection& operator=(const TcpConnection&) = delete;

  void sendPacket(const Packet& packet);
  bool receivePacket(Packet& packet);

 private:
  void sendAll(const uint8_t* data, std::size_t size);

  int socket_ = -1;
};

}  // namespace tcp_station

#endif  // TCP_STATION_TCP_CONNECTION_H
