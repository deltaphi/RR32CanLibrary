#include "RR32Can_config.h"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "RR32Can/Station.h"
#include "TextConfigDataCallback.h"
#include "UdpLogger.h"
#include "UdpProtocol.h"
#include "UdpStationConfig.h"
#include "UdpTxCallback.h"

namespace {

constexpr uint16_t kStationUuid = (RR32Can::enthusiastStationBaseUUID + 12);

RR32Can::CanFrame decodePacket(const udp_station::Packet& packet) {
  RR32Can::CanFrame frame;
  uint8_t identifier[udp_station::kIdentifierSize] = {packet[0], packet[1], packet[2], packet[3]};
  frame.id = RR32Can::Identifier::GetIdentifier(identifier);
  frame.data.dlc = packet[4];
  if (frame.data.dlc > udp_station::kPayloadSize) {
    throw std::runtime_error("Received CAN packet with DLC greater than 8");
  }
  std::memcpy(frame.data.data, packet.data() + udp_station::kIdentifierSize + 1, udp_station::kPayloadSize);
  return frame;
}

// -----------------------------------------------------------------------------
// Experiments: edit this function and rebuild the example. There is no
// runtime command-line or console input by design.
// -----------------------------------------------------------------------------
void runExperiments(RR32Can::Station& station, udp_station::TextConfigDataCallback& configDataCallback) {
  // station.SendSystemGo();
  station.RequestLokListe(&configDataCallback);
  // station.RequestEngineList(0, &configDataCallback);
  //  Add experimental library calls here, for example:
  //  station.SendSystemStop();
  //  station.SendAccessoryPacket(...);
}

}  // namespace

int main() {
  try {
    std::cout << "Sending UDP to " << udp_station_config::kServerHost << ':' << udp_station_config::kServerPort
              << "; listening for broadcasts on 0.0.0.0:" << udp_station_config::kReceivePort << "...\n";
    udp_station::UdpConnection connection(udp_station_config::kServerHost, udp_station_config::kServerPort,
                                          udp_station_config::kReceivePort);
    udp_station::UdpTxCallback txCallback(connection);
    udp_station::TextConfigDataCallback configDataCallback(
        "lokliste-test.cs2", udp_station::TextConfigDataCallback::FileMode::OmitNullCharacters);

    RR32Can::Station::CallbackStruct callbacks;
    callbacks.tx = &txCallback;

    RR32Can::Station station;
    station.begin(kStationUuid, callbacks);

    runExperiments(station, configDataCallback);

    udp_station::Packet packet{};
    while (connection.receivePacket(packet)) {
      const RR32Can::CanFrame frame = decodePacket(packet);
      udp_station::logPacket("RX", packet, frame);
      station.HandlePacket(frame);
      station.loop();
    }

    configDataCallback.closeFile();
    std::cout << "UDP listener stopped.\n";
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }
}
