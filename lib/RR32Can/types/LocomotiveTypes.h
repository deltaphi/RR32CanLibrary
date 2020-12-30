#ifndef __RR32CAN__TYPES__LOCOMOTIVETYPES_H__
#define __RR32CAN__TYPES__LOCOMOTIVETYPES_H__

#include "RR32Can/types/BaseTypes.h"

namespace RR32Can {

/// Engine Direction
enum class EngineDirection { UNCHANGED = 0, FORWARD = 1, REVERSE = 2, CHANGE_DIRECTION = 3, UNKNOWN = 4 };

EngineDirection switchDirection(EngineDirection);

/// Data type to carry engine velocities. Range: 1..1000
using Uid_t = uint32_t;
using Velocity_t = uint16_t;
using EngineAddress_t = uint32_t;
using FunctionBits_t = uint16_t;

}  // namespace RR32Can

#endif  // __RR32CAN__TYPES__LOCOMOTIVETYPES_H__
