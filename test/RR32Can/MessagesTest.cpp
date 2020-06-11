#include "gtest/gtest.h"

#include "RR32Can/Constants.h"
#include "RR32Can/messages/Identifier.h"

TEST(Messages, Identifier_Accessory_Request) {
  uint32_t packetId = 0x00163F15;
  RR32Can::Identifier id;
  id = RR32Can::Identifier::GetIdentifier(packetId);
  EXPECT_EQ(id.prio, 0);
  EXPECT_EQ(id.command, RR32Can::kAccessorySwitch);
  EXPECT_FALSE(id.response);
  EXPECT_EQ(id.hash, 16149);
}

TEST(Messages, Identifier_Accessory_Response) {
  uint32_t packetId = 0x0017271C;
  RR32Can::Identifier id;
  id = RR32Can::Identifier::GetIdentifier(packetId);
  EXPECT_EQ(id.prio, 0);
  EXPECT_EQ(id.command, RR32Can::kAccessorySwitch);
  EXPECT_TRUE(id.response);
  EXPECT_EQ(id.hash, 10012);
}