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

  void initData() { BaseMessage::initData(); }

  Subtype getSubtype() const {
    switch (length()) {
      case 4:
        return Subtype::REQUEST;
      case 5:
        return Subtype::EVENT;
      default:
        return Subtype::RESPONSE;
    }
  }

  void setSubtype(Subtype subtype) {
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

  MachineTurnoutAddress getDeviceId() const {
    uint16_t addr = data_.data[0] << 8 | data_.data[1];
    return MachineTurnoutAddress(addr);
  }

  void setDeviceId(MachineTurnoutAddress addr) const {
    auto value = addr.value();
    data_.data[1] = value;
    data_.data[0] = value >> 8;
  }

  MachineTurnoutAddress getContactId() const {
    uint16_t addr = data_.data[2] << 8 | data_.data[3];
    return MachineTurnoutAddress(addr);
  }

  void setContactId(MachineTurnoutAddress addr) const {
    auto value = addr.value();
    data_.data[3] = value;
    data_.data[2] = value >> 8;
  }
};

}  // namespace RR32Can

#endif  // __RR32CAN__MESSAGES__S88EVENT_H__
