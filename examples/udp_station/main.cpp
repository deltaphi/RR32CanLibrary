#include "RR32Can_config.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "RR32Can/Station.h"
#include "RR32CanEngineDb/LoklisteConsumer.h"
#include "RR32CanEngineDb/util/ConfigDataEndStreamCallback.h"
#include "RR32CanEngineDb/util/ConfigDataStreamParser.h"
#include "UdpLogger.h"
#include "UdpProtocol.h"
#include "UdpStationConfig.h"
#include "UdpTxCallback.h"

namespace {

constexpr uint16_t kStationUuid = (RR32Can::enthusiastStationBaseUUID + 12);
constexpr size_t kNumEngines = 40;

class LokListeCallback : public RR32Can::LokListeConsumerCallback {
 public:
  using EngineArray = std::array<RR32Can::Locomotive, kNumEngines>;

  explicit LokListeCallback(EngineArray& engines) : engines_(engines) {}

  RR32Can::LocomotiveShortInfo* getLokInfoShort(uint32_t index) override {
    return index < engines_.size() ? &engines_[index] : nullptr;
  }

 private:
  EngineArray& engines_;
};

class LokListeEndCallback : public RR32Can::ConfigDataEndStreamCallback {
 public:
  using EngineArray = std::array<RR32Can::Locomotive, kNumEngines>;

  explicit LokListeEndCallback(EngineArray& engines) : engines_(engines) {}

  void streamComplete(RR32Can::ConfigDataConsumer*) override {
    for (size_t index = 0; index < engines_.size(); ++index) {
      std::cout << index << ": " << (engines_[index].isFree() ? "<empty>" : engines_[index].getName()) << '\n';
    }
  }

  void streamAborted(RR32Can::ConfigDataConsumer*) override { std::cout << "Lokliste download aborted.\n"; }

 private:
  EngineArray& engines_;
};

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
void runExperiments(RR32Can::Station& station, RR32Can::ConfigDataStreamParser& parser) {
  // station.SendSystemGo();
  station.RequestLokListe(&parser);
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
    std::array<RR32Can::Locomotive, kNumEngines> engines;
    LokListeCallback lokListeCallback(engines);
    LokListeEndCallback lokListeEndCallback(engines);
    RR32Can::LoklisteConsumer lokListeConsumer(lokListeCallback);
    lokListeConsumer.setStreamEndCallback(&lokListeEndCallback);
    RR32Can::ConfigDataStreamParser parser;
    parser.startStream(&lokListeConsumer);

    RR32Can::Station::CallbackStruct callbacks;
    callbacks.tx = &txCallback;

    RR32Can::Station station;
    station.begin(kStationUuid, callbacks);

    runExperiments(station, parser);

    udp_station::Packet packet{};
    while (connection.receivePacket(packet)) {
      const RR32Can::CanFrame frame = decodePacket(packet);
      udp_station::logPacket("RX", packet, frame);
      station.HandlePacket(frame);
      station.loop();
    }

    std::cout << "UDP listener stopped.\n";
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }
}
