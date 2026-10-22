#ifndef UDP_STATION_UDP_CONNECTION_H
#define UDP_STATION_UDP_CONNECTION_H

#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

#include "UdpProtocol.h"

namespace udp_station {

class UdpConnection {
 public:
  UdpConnection(const char* host, const char* port, uint16_t receivePort);
  ~UdpConnection();

  UdpConnection(const UdpConnection&) = delete;
  UdpConnection& operator=(const UdpConnection&) = delete;

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

}  // namespace udp_station

#endif  // UDP_STATION_UDP_CONNECTION_H
