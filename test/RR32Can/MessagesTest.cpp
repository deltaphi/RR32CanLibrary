#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "RR32Can/Constants.h"
#include "RR32Can/messages/Identifier.h"
#include "RR32Can/messages/TurnoutPacket.h"

TEST(Messages, Identifier_Accessory_Request) {
  uint32_t packetId = 0x00163F15;
  RR32Can::Identifier id;
  id = RR32Can::Identifier::GetIdentifier(packetId);
  EXPECT_EQ(id.prio_, 0);
  EXPECT_EQ(id.command_, RR32Can::kAccessorySwitch);
  EXPECT_FALSE(id.response_);
  EXPECT_EQ(id.hash_, 16149);
}

TEST(Messages, Identifier_Accessory_Response) {
  uint32_t packetId = 0x0017271C;
  RR32Can::Identifier id;
  id = RR32Can::Identifier::GetIdentifier(packetId);
  EXPECT_EQ(id.prio_, 0);
  EXPECT_EQ(id.command_, RR32Can::kAccessorySwitch);
  EXPECT_TRUE(id.response_);
  EXPECT_EQ(id.hash_, 10012);
}

TEST(Messages, HumanTurnoutAddrEqual) {
  RR32Can::HumanTurnoutAddress addr1{5};
  RR32Can::HumanTurnoutAddress addr2{5};
  RR32Can::HumanTurnoutAddress addr3{6};

  EXPECT_EQ(addr1, addr2);
  EXPECT_NE(addr1, addr3);
  EXPECT_NE(addr2, addr3);
}

TEST(Messages, MachineTurnoutAddrEqual) {
  RR32Can::MachineTurnoutAddress addr4{4};
  RR32Can::MachineTurnoutAddress addr5{5};
  RR32Can::MachineTurnoutAddress addr6{6};
  RR32Can::MachineTurnoutAddress addr7{6};

  EXPECT_EQ(addr6, addr7);
  EXPECT_NE(addr4, addr5);
  EXPECT_NE(addr4, addr6);
  EXPECT_NE(addr4, addr7);
  EXPECT_NE(addr5, addr6);
  EXPECT_NE(addr5, addr7);
}

TEST(Messages, MixedTurnoutAddrEqual) {
  RR32Can::HumanTurnoutAddress addr1{4};
  RR32Can::HumanTurnoutAddress addr2{5};
  RR32Can::HumanTurnoutAddress addr3{6};

  RR32Can::MachineTurnoutAddress addr4{4};
  RR32Can::MachineTurnoutAddress addr5{5};
  RR32Can::MachineTurnoutAddress addr6{6};

  EXPECT_NE(addr1, addr4);
  EXPECT_NE(addr2, addr5);
  EXPECT_NE(addr3, addr6);

  EXPECT_EQ(addr2, addr4);
  EXPECT_EQ(addr3, addr5);
}

TEST(Messages, TurnoutPacket) {
  {
    RR32Can::TurnoutPacket packet;
    packet.locid = RR32Can::HumanTurnoutAddress(11);
    packet.position = RR32Can::TurnoutDirection::GREEN;
    packet.power = 1;

    RR32Can::Data busData;
    packet.serialize(busData);

    ASSERT_EQ(busData.dlc, 6);
    EXPECT_THAT(busData.data, ::testing::ElementsAre(0, 0, 0, 10, 1, 1, 0, 0, 0));

    RR32Can::TurnoutPacket recvPacket = RR32Can::TurnoutPacket::FromCanPacket(busData);

    EXPECT_EQ(packet, recvPacket);
  }

  {
    RR32Can::TurnoutPacket packet;
    packet.locid = RR32Can::HumanTurnoutAddress(1);
    packet.position = RR32Can::TurnoutDirection::YELLOW;
    packet.power = 0;

    RR32Can::Data busData;
    packet.serialize(busData);

    ASSERT_EQ(busData.dlc, 6);
    EXPECT_THAT(busData.data, ::testing::ElementsAre(0, 0, 0, 0, 2, 0, 0, 0, 0));

    RR32Can::TurnoutPacket recvPacket = RR32Can::TurnoutPacket::FromCanPacket(busData);

    EXPECT_EQ(packet, recvPacket);
  }
}

TEST(Messages, TurnoutPacket_AddrProto) {
  RR32Can::TurnoutPacket packet;
  packet.locid = RR32Can::HumanTurnoutAddress(0x3012);  // MM2 addr
  EXPECT_EQ(packet.getRailProtocol(), RR32Can::RailProtocol::MM1);
  EXPECT_EQ(packet.locid.getNumericAddress(), RR32Can::HumanTurnoutAddress(0x12));

  packet.locid = RR32Can::HumanTurnoutAddress(0x3812);  // DCC Addr
  EXPECT_EQ(packet.getRailProtocol(), RR32Can::RailProtocol::DCC);
  EXPECT_EQ(packet.locid.getNumericAddress(), RR32Can::HumanTurnoutAddress(0x12));

  packet.locid = RR32Can::HumanTurnoutAddress(0x2812);  // SX Addr
  EXPECT_EQ(packet.getRailProtocol(), RR32Can::RailProtocol::SX1);
  EXPECT_EQ(packet.locid.getNumericAddress(), RR32Can::HumanTurnoutAddress(0x12));
}
