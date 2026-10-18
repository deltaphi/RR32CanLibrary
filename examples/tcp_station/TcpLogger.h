#ifndef TCP_STATION_TCP_LOGGER_H
#define TCP_STATION_TCP_LOGGER_H

#include "TcpProtocol.h"

namespace RR32Can {
struct CanFrame;
}

namespace tcp_station {

void logPacket(const char* direction, const Packet& packet, const RR32Can::CanFrame& frame);

}  // namespace tcp_station

#endif  // TCP_STATION_TCP_LOGGER_H
