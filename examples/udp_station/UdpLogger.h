#ifndef UDP_STATION_UDP_LOGGER_H
#define UDP_STATION_UDP_LOGGER_H

#include "UdpProtocol.h"

namespace RR32Can {
struct CanFrame;
}

namespace udp_station {

void logPacket(const char* direction, const Packet& packet, const RR32Can::CanFrame& frame);

}  // namespace udp_station

#endif  // UDP_STATION_UDP_LOGGER_H
