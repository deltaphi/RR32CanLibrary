#include "UdpConnection.h"

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <algorithm>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace udp_station {
namespace {

#ifdef _WIN32
class WinsockRuntime {
 public:
  WinsockRuntime() {
    WSADATA data{};
    const int result = WSAStartup(MAKEWORD(2, 2), &data);
    if (result != 0) {
      throw std::runtime_error("WSAStartup failed: " + std::to_string(result));
    }
  }

  ~WinsockRuntime() { WSACleanup(); }

  WinsockRuntime(const WinsockRuntime&) = delete;
  WinsockRuntime& operator=(const WinsockRuntime&) = delete;
};

void ensureSocketsInitialized() {
  static const WinsockRuntime runtime;
  (void)runtime;
}

bool isInvalidSocket(SOCKET socket) { return socket == INVALID_SOCKET; }

void closeSocket(SOCKET socket) { closesocket(socket); }

const char* addressError(int error) { return gai_strerrorA(error); }

std::string socketError() { return std::to_string(WSAGetLastError()); }
#else
void ensureSocketsInitialized() {}

bool isInvalidSocket(int socket) { return socket < 0; }

void closeSocket(int socket) { close(socket); }

const char* addressError(int error) { return gai_strerror(error); }

std::string socketError() { return std::strerror(errno); }

bool waitForSocketOrStdin(int socket) {
  while (true) {
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(socket, &readSet);
    FD_SET(STDIN_FILENO, &readSet);

    const int result = select(std::max(socket, STDIN_FILENO) + 1, &readSet, nullptr, nullptr, nullptr);
    if (result < 0) {
      if (errno == EINTR) {
        continue;
      }
      throw std::runtime_error(std::string("select: ") + socketError());
    }

    if (FD_ISSET(STDIN_FILENO, &readSet)) {
      char input;
      const ssize_t count = read(STDIN_FILENO, &input, sizeof(input));
      if (count == 0) {
        return false;
      }
      if (count < 0 && errno != EINTR) {
        throw std::runtime_error(std::string("read stdin: ") + socketError());
      }
    }

    if (FD_ISSET(socket, &readSet)) {
      return true;
    }
  }
}
#endif

}  // namespace

UdpConnection::UdpConnection(const char* host, const char* port, uint16_t receivePort) {
  ensureSocketsInitialized();

  struct addrinfo hints{};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = IPPROTO_UDP;

  struct addrinfo* addresses = nullptr;
  const int result = getaddrinfo(host, port, &hints, &addresses);
  if (result != 0) {
    throw std::runtime_error(std::string("getaddrinfo: ") + addressError(result));
  }

  for (const struct addrinfo* address = addresses; address != nullptr; address = address->ai_next) {
    socket_ = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
    if (isInvalidSocket(socket_)) {
      continue;
    }
    const auto destination = *reinterpret_cast<const sockaddr_in*>(address->ai_addr);
    sockaddr_in local{};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(receivePort);
    const int broadcast = 1;
    if (setsockopt(socket_, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char*>(&broadcast),
                   sizeof(broadcast)) == 0 &&
        bind(socket_, reinterpret_cast<const sockaddr*>(&local), sizeof(local)) == 0) {
      destination_ = destination;
      break;
    }
    closeSocket(socket_);
#ifdef _WIN32
    socket_ = INVALID_SOCKET;
#else
    socket_ = -1;
#endif
  }

  freeaddrinfo(addresses);
  if (isInvalidSocket(socket_)) {
    throw std::runtime_error(std::string("Unable to open UDP socket on port ") + std::to_string(receivePort) + ": " +
                             socketError());
  }
}

UdpConnection::~UdpConnection() {
  if (!isInvalidSocket(socket_)) {
    closeSocket(socket_);
  }
}

bool UdpConnection::receivePacket(Packet& packet) {
  // One extra byte lets us reject oversized datagrams even when recvfrom truncates them.
  std::array<uint8_t, kPacketSize + 1> datagram{};
  while (true) {
#ifndef _WIN32
    if (!waitForSocketOrStdin(socket_)) {
      return false;
    }
#endif
#ifdef _WIN32
    const int count =
        recvfrom(socket_, reinterpret_cast<char*>(datagram.data()), static_cast<int>(datagram.size()), 0, nullptr, nullptr);
#else
    const ssize_t count = recvfrom(socket_, datagram.data(), datagram.size(), 0, nullptr, nullptr);
#endif
    if (count < 0) {
#ifdef _WIN32
      if (WSAGetLastError() == WSAEINTR || WSAGetLastError() == WSAEMSGSIZE) {
#else
      if (errno == EINTR) {
#endif
        continue;
      }
      throw std::runtime_error(std::string("recvfrom: ") + socketError());
    }
    if (static_cast<std::size_t>(count) == packet.size()) {
      std::memcpy(packet.data(), datagram.data(), packet.size());
      return true;
    }
  }
}

void UdpConnection::sendPacket(const Packet& packet) {
  while (true) {
#ifdef _WIN32
    const int count = sendto(socket_, reinterpret_cast<const char*>(packet.data()), static_cast<int>(packet.size()), 0,
                             reinterpret_cast<const sockaddr*>(&destination_), sizeof(destination_));
#else
    const ssize_t count = sendto(socket_, packet.data(), packet.size(), 0,
                                 reinterpret_cast<const sockaddr*>(&destination_), sizeof(destination_));
#endif
    if (count < 0) {
#ifdef _WIN32
      if (WSAGetLastError() == WSAEINTR) {
#else
      if (errno == EINTR) {
#endif
        continue;
      }
      throw std::runtime_error(std::string("sendto: ") + socketError());
    }
    if (static_cast<std::size_t>(count) != packet.size()) {
      throw std::runtime_error("sendto did not send a complete CAN datagram");
    }
    return;
  }
}

}  // namespace udp_station
