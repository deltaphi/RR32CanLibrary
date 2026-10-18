#include "TcpConnection.h"

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>

namespace tcp_station {

TcpConnection::TcpConnection(const char* host, const char* port) {
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

TcpConnection::~TcpConnection() {
  if (socket_ >= 0) {
    close(socket_);
  }
}

void TcpConnection::sendPacket(const Packet& packet) { sendAll(packet.data(), packet.size()); }

bool TcpConnection::receivePacket(Packet& packet) {
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

void TcpConnection::sendAll(const uint8_t* data, std::size_t size) {
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

}  // namespace tcp_station
