#include "RR32Can_config.h"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "RR32Can/Station.h"
#include "TcpLogger.h"
#include "TcpProtocol.h"
#include "TcpStationConfig.h"
#include "TcpTxCallback.h"
#include "TextConfigDataCallback.h"

namespace {

constexpr uint16_t kStationUuid = (RR32Can::enthusiastStationBaseUUID + 12);

RR32Can::CanFrame decodePacket(const tcp_station::Packet& packet) {
  RR32Can::CanFrame frame;
  uint8_t identifier[tcp_station::kIdentifierSize] = {packet[0], packet[1], packet[2], packet[3]};
  frame.id = RR32Can::Identifier::GetIdentifier(identifier);
  frame.data.dlc = packet[4];
  if (frame.data.dlc > tcp_station::kPayloadSize) {
    throw std::runtime_error("Received CAN packet with DLC greater than 8");
  }
  std::memcpy(frame.data.data, packet.data() + tcp_station::kIdentifierSize + 1, tcp_station::kPayloadSize);
  return frame;
}

// -----------------------------------------------------------------------------
// Experiments: edit this function and rebuild the example. There is no
// runtime command-line or console input by design.
// -----------------------------------------------------------------------------
void runExperiments(RR32Can::Station& station, tcp_station::TextConfigDataCallback& configDataCallback) {
  // station.SendSystemGo();
  station.RequestLokListe(&configDataCallback);
  //station.RequestEngineList(0, &configDataCallback);
  // Add experimental library calls here, for example:
  // station.SendSystemStop();
  // station.SendAccessoryPacket(...);
}

}  // namespace

int main() {
  try {
    std::cout << "Connecting to " << tcp_station_config::kServerHost << ':' << tcp_station_config::kServerPort
              << "...\n";
    tcp_station::TcpConnection connection(tcp_station_config::kServerHost, tcp_station_config::kServerPort);
    tcp_station::TcpTxCallback txCallback(connection);
    tcp_station::TextConfigDataCallback configDataCallback;

    RR32Can::Station::CallbackStruct callbacks;
    callbacks.tx = &txCallback;

    RR32Can::Station station;
    station.begin(kStationUuid, callbacks);

    runExperiments(station, configDataCallback);

    tcp_station::Packet packet{};
    while (connection.receivePacket(packet)) {
      const RR32Can::CanFrame frame = decodePacket(packet);
      tcp_station::logPacket("RX", packet, frame);
      station.HandlePacket(frame);
      station.loop();
    }

    std::cout << "TCP connection closed by the server.\n";
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }
}
