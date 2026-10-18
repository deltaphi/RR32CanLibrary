#ifndef TCP_STATION_TCP_CONNECTION_H
#define TCP_STATION_TCP_CONNECTION_H

#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

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
  sockaddr_in destination_{};

#ifdef _WIN32
  SOCKET socket_ = INVALID_SOCKET;
#else
  int socket_ = -1;
#endif
};

}  // namespace tcp_station

#endif  // TCP_STATION_TCP_CONNECTION_H
