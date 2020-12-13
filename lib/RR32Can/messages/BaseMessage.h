#ifndef __RR32CAN__MESSAGES__BASEMESSAGE_H__
#define __RR32CAN__MESSAGES__BASEMESSAGE_H__

#include "RR32Can/messages/Data.h"

namespace RR32Can {

/*
 * \brief Class BaseMessage
 */
class BaseMessage {
 public:
  BaseMessage(RR32Can::Data& data) : data_(data){};

  uint8_t length() const { return data_.dlc; }

 protected:
  /**
   * Zeroes the entire message.
   */
  void initData() {
    data_.dlc = 0;
    memset(data_.data, 0, sizeof(&(data_.data)));
  }

  

  RR32Can::Data& data_;
};

}  // namespace RR32Can

#endif  // __RR32CAN__MESSAGES__BASEMESSAGE_H__
