#include "RR32Can_config.h"

#include <array>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include "RR32Can/Station.h"

namespace {

constexpr const char* kServerHost = "127.0.0.1";
constexpr const char* kServerPort = "15731";
constexpr uint16_t kStationUuid = 0;
constexpr std::size_t kPacketSize = 13;
constexpr std::size_t kIdentifierSize = 4;
constexpr std::size_t kPayloadSize = 8;

using Packet = std::array<uint8_t, kPacketSize>;

class TcpConnection {
 public:
  TcpConnection(const char* host, const char* port) {
    struct addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo* addresses = nullptr;
    const int result = getaddrinfo(host, port, &hints, &addresses);
    if (result != 0) {
      throw std::runtime_error(std::string("getaddrinfo: ") + gai_strerror(result));
    }

    for (const struct addrinfo* address = addresses; address != nullptr; address = address->ai_next) {
      socket_ = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
      if (socket_ < 0) {
        continue;
      }
      if (connect(socket_, address->ai_addr, address->ai_addrlen) == 0) {
        break;
      }
      close(socket_);
      socket_ = -1;
    }

    freeaddrinfo(addresses);
    if (socket_ < 0) {
      throw std::runtime_error(std::string("Unable to connect to ") + host + ":" + port + ": " + std::strerror(errno));
    }
  }

  ~TcpConnection() {
    if (socket_ >= 0) {
      close(socket_);
    }
  }

  TcpConnection(const TcpConnection&) = delete;
  TcpConnection& operator=(const TcpConnection&) = delete;

  void sendPacket(const Packet& packet) {
    sendAll(packet.data(), packet.size());
  }

  bool receivePacket(Packet& packet) {
    std::size_t received = 0;
    while (received < packet.size()) {
      const ssize_t count = recv(socket_, packet.data() + received, packet.size() - received, 0);
      if (count == 0) {
        return false;
      }
      if (count < 0) {
        if (errno == EINTR) {
          continue;
        }
        throw std::runtime_error(std::string("recv: ") + std::strerror(errno));
      }
      received += static_cast<std::size_t>(count);
    }
    return true;
  }

 private:
  void sendAll(const uint8_t* data, std::size_t size) {
    std::size_t sent = 0;
    while (sent < size) {
      const ssize_t count = send(socket_, data + sent, size - sent, 0);
      if (count < 0) {
        if (errno == EINTR) {
          continue;
        }
        throw std::runtime_error(std::string("send: ") + std::strerror(errno));
      }
      if (count == 0) {
        throw std::runtime_error("send returned zero bytes");
      }
      sent += static_cast<std::size_t>(count);
    }
  }

  int socket_ = -1;
};

void printHex(const Packet& packet) {
  for (const uint8_t byte : packet) {
    std::printf(" %02X", static_cast<unsigned>(byte));
  }
}

RR32Can::CanFrame decodePacket(const Packet& packet) {
  RR32Can::CanFrame frame;
  uint8_t identifier[kIdentifierSize] = {packet[0], packet[1], packet[2], packet[3]};
  frame.id = RR32Can::Identifier::GetIdentifier(identifier);
  frame.data.dlc = packet[4];
  if (frame.data.dlc > kPayloadSize) {
    throw std::runtime_error("Received CAN packet with DLC greater than 8");
  }
  std::memcpy(frame.data.data, packet.data() + kIdentifierSize + 1, kPayloadSize);
  return frame;
}

void logPacket(const char* direction, const Packet& packet, const RR32Can::CanFrame& frame) {
  std::cout << direction << " TCP packet:";
  printHex(packet);
  std::cout << "\n  ";
  frame.id.printAll();
  std::cout << ", Payload:";
  frame.data.printAsHex();
  std::cout << '\n';
}

class TcpTxCallback : public RR32Can::callback::TxCbk {
 public:
  explicit TcpTxCallback(TcpConnection& connection) : connection_(connection) {}

  void SendPacket(const RR32Can::CanFrame& frame) override {
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

 private:
  TcpConnection& connection_;
};

// -----------------------------------------------------------------------------
// Experiments: edit this function and rebuild the example. There is no
// runtime command-line or console input by design.
// -----------------------------------------------------------------------------
void runExperiments(RR32Can::Station& station) {
  station.SendSystemGo();

  // Add experimental library calls here, for example:
  // station.SendSystemStop();
  // station.SendAccessoryPacket(...);
}

}  // namespace

int main() {
  try {
    std::cout << "Connecting to " << kServerHost << ':' << kServerPort << "...\n";
    TcpConnection connection(kServerHost, kServerPort);
    TcpTxCallback txCallback(connection);

    RR32Can::Station::CallbackStruct callbacks;
    callbacks.tx = &txCallback;

    RR32Can::Station station;
    station.begin(kStationUuid, callbacks);

    runExperiments(station);

    Packet packet{};
    while (connection.receivePacket(packet)) {
      const RR32Can::CanFrame frame = decodePacket(packet);
      logPacket("RX", packet, frame);
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
