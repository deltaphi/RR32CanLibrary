#ifndef __RR32CAN__MESSAGES__S88EVENT_H__
#define __RR32CAN__MESSAGES__S88EVENT_H__

#include "RR32Can/Types.h"
#include "RR32Can/messages/BaseMessage.h"

namespace RR32Can {

/*
 * \brief Class S88Event
 */
class S88Event : public BaseMessage {
 public:
  enum class Subtype { REQUEST, EVENT, RESPONSE };
  enum class State { OPEN = 0, CLOSED = 1 };

  using BaseMessage::BaseMessage;
  void initData() { BaseMessage::initData(); }

  constexpr Subtype getSubtype() const {
    switch (length()) {
      case 4:
        return Subtype::REQUEST;
      case 5:
        return Subtype::EVENT;
      default:
        return Subtype::RESPONSE;
    }
  }

  constexpr void setSubtype(Subtype subtype) {
    switch (subtype) {
      case Subtype::REQUEST:
        data_.dlc = 4;
        break;
      case Subtype::EVENT:
        data_.dlc = 5;
        break;
      case Subtype::RESPONSE:
        data_.dlc = 8;
        break;
    }
  }

  constexpr MachineTurnoutAddress getDeviceId() const {
    uint16_t addr = data_.data[0] << 8 | data_.data[1];
    return MachineTurnoutAddress(addr);
  }

  constexpr void setDeviceId(MachineTurnoutAddress addr) const {
    auto value = addr.value();
    data_.data[1] = value;
    data_.data[0] = value >> 8;
  }

  constexpr MachineTurnoutAddress getContactId() const {
    uint16_t addr = data_.data[2] << 8 | data_.data[3];
    return MachineTurnoutAddress(addr);
  }

  constexpr void setContactId(MachineTurnoutAddress addr) const {
    auto value = addr.value();
    data_.data[3] = value;
    data_.data[2] = value >> 8;
  }

  constexpr void setStates(State oldState, State newState) {
    data_.data[4] = static_cast<uint8_t>(oldState);
    data_.data[5] = static_cast<uint8_t>(newState);
  }

  constexpr State getOldState() const { return static_cast<State>(data_.data[4]); }
  constexpr State getNewState() const { return static_cast<State>(data_.data[5]); }

  constexpr void setTime(uint16_t time) {
    data_.data[7] = time;
    data_.data[6] = time >> 8;
  }

  constexpr uint16_t getTime() const {
    uint16_t time = (data_.data[6] << 8) | data_.data[7];
    return time;
  }
};

}  // namespace RR32Can

#endif  // __RR32CAN__MESSAGES__S88EVENT_H__
