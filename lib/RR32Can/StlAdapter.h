#ifndef __RR32CAN__STLADAPTER_H__
#define __RR32CAN__STLADAPTER_H__

#if defined(ARDUINO) && (defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__))
#include "printf.h"
#else
// Assume presence of an STL
#include <cstdio>
#endif

#endif  // __RR32CAN__STLADAPTER_H__
