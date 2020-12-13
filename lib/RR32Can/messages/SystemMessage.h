#ifndef __RR32CAN__MESSAGES__SYSTEMMESSAGE_H__
#define __RR32CAN__MESSAGES__SYSTEMMESSAGE_H__

#include "RR32Can/messages/BaseMessage.h"

namespace RR32Can {

/*
 * \brief Class SystemMessage
 */
class SystemMessage : public BaseMessage {
 public:
  using BaseMessage::BaseMessage;

  void initData() {
    BaseMessage::initData();
    data_.dlc = 5;
  }

  SystemSubcommand getSubcommand() const { return static_cast<SystemSubcommand>(data_.data[4]); }
  void setSubcommand(SystemSubcommand command) { data_.data[4] = static_cast<CommandByte_t>(command); }

  Uid_t getUid() const {
    return (static_cast<Uid_t>(data_.data[0]) << 24) | (static_cast<Uid_t>(data_.data[1]) << 16) |
           (static_cast<Uid_t>(data_.data[2]) << 8) | (data_.data[3]);
  }

  void setUid(RR32Can::Uid_t uid) {
    data_.data[3] = uid;
    data_.data[2] = uid >> 8;
    data_.data[1] = uid >> 16;
    data_.data[0] = uid >> 24;
  }

  LocId_t getLocid() const { return data_.getLocid(); }

  void setLocid(LocId_t locid) { data_.setLocid(locid); }
};

}  // namespace RR32Can

#endif  // __RR32CAN__MESSAGES__SYSTEMMESSAGE_H__
