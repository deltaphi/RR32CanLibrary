#ifndef UDP_STATION_UDP_PROTOCOL_H
#define UDP_STATION_UDP_PROTOCOL_H

#include <array>
#include <cstdint>

namespace udp_station {

constexpr std::size_t kPacketSize = 13;
constexpr std::size_t kIdentifierSize = 4;
constexpr std::size_t kPayloadSize = 8;

using Packet = std::array<uint8_t, kPacketSize>;

}  // namespace udp_station

#endif  // UDP_STATION_UDP_PROTOCOL_H
