#include "UdpLogger.h"

#include <cstdio>
#include <iostream>

#include "RR32Can/messages/CanFrame.h"

namespace udp_station {
namespace {

void printHex(const Packet& packet) {
  for (const uint8_t byte : packet) {
    std::printf(" %02X", static_cast<unsigned>(byte));
  }
}

}  // namespace

void logPacket(const char* direction, const Packet& packet, const RR32Can::CanFrame& frame) {
  std::cout << direction << " UDP packet:";
  printHex(packet);
  std::cout << "\n  ";
  frame.id.printAll();
  std::cout << ", Payload:";
  frame.data.printAsHex();
  std::cout << '\n';
}

}  // namespace udp_station
