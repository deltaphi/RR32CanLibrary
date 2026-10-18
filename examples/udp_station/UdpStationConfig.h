#ifndef UDP_STATION_CONFIG_H
#define UDP_STATION_CONFIG_H

#include <cstdint>

namespace udp_station_config {

constexpr const char* kServerHost = "srseii.local";
constexpr const char* kServerPort = "15731";
constexpr uint16_t kReceivePort = 15730;

}  // namespace udp_station_config

#endif  // UDP_STATION_CONFIG_H
