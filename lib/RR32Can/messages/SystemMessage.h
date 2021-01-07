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

  constexpr void initData() {
    BaseMessage::initData();
    data_.dlc = 5;
  }

  constexpr SystemSubcommand getSubcommand() const { return static_cast<SystemSubcommand>(data_.data[4]); }
  constexpr void setSubcommand(SystemSubcommand command) { data_.data[4] = static_cast<CommandByte_t>(command); }

  constexpr LocId_t getLocid() const { return data_.getLocid(); }
  constexpr void setLocid(LocId_t locid) { data_.setLocid(locid); }
};

}  // namespace RR32Can

#endif  // __RR32CAN__MESSAGES__SYSTEMMESSAGE_H__
