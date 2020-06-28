#include "gtest/gtest.h"

#include "RR32Can/Constants.h"

namespace RR32Can {

TEST(Constants, AccessoryMM) {
  EXPECT_EQ(kMMAccessoryAddrStart, getAccessoryMask(RailProtocol::MM1));
  EXPECT_EQ(kMMAccessoryAddrStart, getAccessoryMask(RailProtocol::MM2));
  EXPECT_EQ(kMMAccessoryAddrStart, getAccessoryMask(RailProtocol::MFX));
}

TEST(Constants, AccessorySX) {
  EXPECT_EQ(kSX1AccessoryAddrStart, getAccessoryMask(RailProtocol::SX1));
  EXPECT_EQ(kSX1AccessoryAddrStart, getAccessoryMask(RailProtocol::SX2));
}

TEST(Constants, AccessoryDCC) { EXPECT_EQ(kDCCAccessoryAddrStart, getAccessoryMask(RailProtocol::DCC)); }

}  // namespace RR32Can
