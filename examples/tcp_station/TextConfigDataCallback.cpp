#include "TextConfigDataCallback.h"

#include <iostream>

namespace tcp_station {

void TextConfigDataCallback::startStream() { streamState = StreamState::WAITING_FIRST_PACKET; }

void TextConfigDataCallback::addMessage(const RR32Can::Data& data) {
  data.printAsText();
  std::cout << '\n';
}

void TextConfigDataCallback::reset() { streamState = StreamState::IDLE; }

}  // namespace tcp_station
