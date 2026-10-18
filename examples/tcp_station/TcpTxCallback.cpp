#include "TcpTxCallback.h"

#include <cstring>
#include <stdexcept>

#include "TcpLogger.h"

namespace tcp_station {

TcpTxCallback::TcpTxCallback(TcpConnection& connection) : connection_(connection) {}

void TcpTxCallback::SendPacket(const RR32Can::CanFrame& frame) {
  if (frame.data.dlc > kPayloadSize) {
    throw std::runtime_error("Attempted to send CAN packet with DLC greater than 8");
  }

  Packet packet{};
  const uint32_t identifier = frame.id.makeIdentifier();
  packet[0] = static_cast<uint8_t>(identifier >> 24);
  packet[1] = static_cast<uint8_t>(identifier >> 16);
  packet[2] = static_cast<uint8_t>(identifier >> 8);
  packet[3] = static_cast<uint8_t>(identifier);
  packet[4] = frame.data.dlc;
  std::memcpy(packet.data() + kIdentifierSize + 1, frame.data.data, kPayloadSize);

  logPacket("TX", packet, frame);
  connection_.sendPacket(packet);
}

}  // namespace tcp_station
