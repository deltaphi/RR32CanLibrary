#ifndef __RR32CAN__MESSAGES__CANFRAME_H__
#define __RR32CAN__MESSAGES__CANFRAME_H__

#include "RR32Can/messages/Data.h"
#include "RR32Can/messages/Identifier.h"

namespace RR32Can {

/*
 * \brief Class CanFrame
 */
struct CanFrame {
 public:
  Identifier id;
  Data data;

  bool operator==(const CanFrame& other) const { return id == other.id && data == other.data; }
};

}  // namespace RR32Can

#endif  // __RR32CAN__MESSAGES__CANFRAME_H__
