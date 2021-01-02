#ifndef __DATA__CONFIGDATA_H__
#define __DATA__CONFIGDATA_H__

#include <cstdint>

namespace data {

constexpr uint8_t testData1NumChunks = 9;
extern char testData1[testData1NumChunks][8 + 1];

constexpr uint8_t testData2NumChunks = 141;
extern char testData2[][8];

constexpr uint8_t testData3NumChunks = 141;
extern char testData3[][8];

}  // namespace data

#endif  // __DATA__CONFIGDATA_H__
