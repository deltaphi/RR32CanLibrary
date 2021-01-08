#ifndef __RR32CAN__TYPES__BASETYPES_H__
#define __RR32CAN__TYPES__BASETYPES_H__

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

namespace RR32Can {

/// Rail communication protocols.
enum class RailProtocol { MM1, MM2, MFX, SX1, SX2, DCC, UNKNOWN };

using LocId_t = uint32_t;

using CommandByte_t = uint8_t;

/// State of the overall System
enum class SystemState { UNKNOWN = 0, ON, OFF };

enum class ConfigDataStreamType {
  NONE = 0,
  LOKOMOTIVE,    /* Database of all engines */
  MAGNETARTIKEL, /* Database of all configured accessories */
  FAHRSTRASSE,   /* Database of all configured multi-accessory paths */
  GLEISBILD,     /* Track layout */
  LOKINFO,       /* Information about a single engine sufficient for a slave device */
  LOKNAMEN,      /* Information about names of engines known to the master controller
                  */
  MAGINFO,
  LOKDB,
  LDBVER
};

enum class SensorState { OPEN = 0, CLOSED = 1 };

}  // namespace RR32Can

#endif  // __RR32CAN__TYPES__BASETYPES_H__
