#ifndef TCP_STATION_TCP_PROTOCOL_H
#define TCP_STATION_TCP_PROTOCOL_H

#include <array>
#include <cstdint>

namespace tcp_station {

constexpr std::size_t kPacketSize = 13;
constexpr std::size_t kIdentifierSize = 4;
constexpr std::size_t kPayloadSize = 8;

using Packet = std::array<uint8_t, kPacketSize>;

}  // namespace tcp_station

#endif  // TCP_STATION_TCP_PROTOCOL_H
