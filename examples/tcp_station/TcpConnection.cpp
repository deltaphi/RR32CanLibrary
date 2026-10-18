#include "TcpConnection.h"

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace tcp_station {
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
#endif

}  // namespace

TcpConnection::TcpConnection(const char* host, const char* port) {
  ensureSocketsInitialized();

  struct addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

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
    if (connect(socket_, address->ai_addr, address->ai_addrlen) == 0) {
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
    throw std::runtime_error(std::string("Unable to connect to ") + host + ":" + port + ": " + socketError());
  }
}

TcpConnection::~TcpConnection() {
  if (!isInvalidSocket(socket_)) {
    closeSocket(socket_);
  }
}

void TcpConnection::sendPacket(const Packet& packet) { sendAll(packet.data(), packet.size()); }

bool TcpConnection::receivePacket(Packet& packet) {
  std::size_t received = 0;
  while (received < packet.size()) {
#ifdef _WIN32
    const int count = recv(socket_, packet.data() + received, static_cast<int>(packet.size() - received), 0);
#else
    const ssize_t count = recv(socket_, packet.data() + received, packet.size() - received, 0);
#endif
    if (count == 0) {
      return false;
    }
    if (count < 0) {
#ifdef _WIN32
      if (WSAGetLastError() == WSAEINTR) {
#else
      if (errno == EINTR) {
#endif
        continue;
      }
      throw std::runtime_error(std::string("recv: ") + socketError());
    }
    received += static_cast<std::size_t>(count);
  }
  return true;
}

void TcpConnection::sendAll(const uint8_t* data, std::size_t size) {
  std::size_t sent = 0;
  while (sent < size) {
#ifdef _WIN32
    const int count =
        send(socket_, data + sent,
             static_cast<int>(std::min(size - sent, static_cast<std::size_t>(std::numeric_limits<int>::max()))), 0);
#else
    const ssize_t count = send(socket_, data + sent, size - sent, 0);
#endif
    if (count < 0) {
#ifdef _WIN32
      if (WSAGetLastError() == WSAEINTR) {
#else
      if (errno == EINTR) {
#endif
        continue;
      }
      throw std::runtime_error(std::string("send: ") + socketError());
    }
    if (count == 0) {
      throw std::runtime_error("send returned zero bytes");
    }
    sent += static_cast<std::size_t>(count);
  }
}

}  // namespace tcp_station
