#include "gtest/gtest.h"

#include "RR32Can/Constants.h"

namespace RR32Can {

TEST(Constants, AccessoryMM) {
  EXPECT_EQ(kMMAccessoryAddrStart, getAccessoryLocIdMask(RailProtocol::MM1));
  EXPECT_EQ(kMMAccessoryAddrStart, getAccessoryLocIdMask(RailProtocol::MM2));
  EXPECT_EQ(kMMAccessoryAddrStart, getAccessoryLocIdMask(RailProtocol::MFX));
}

TEST(Constants, AccessorySX) {
  EXPECT_EQ(kSX1AccessoryAddrStart, getAccessoryLocIdMask(RailProtocol::SX1));
  EXPECT_EQ(kSX1AccessoryAddrStart, getAccessoryLocIdMask(RailProtocol::SX2));
}

TEST(Constants, AccessoryDCC) { EXPECT_EQ(kDCCAccessoryAddrStart, getAccessoryLocIdMask(RailProtocol::DCC)); }

}  // namespace RR32Can
