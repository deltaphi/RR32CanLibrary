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
    RR32Can::Data busData;
    RR32Can::TurnoutPacket packet(busData);
    packet.initData();
    packet.setLocid(RR32Can::HumanTurnoutAddress(11));
    packet.setDirection(RR32Can::TurnoutDirection::GREEN);
    packet.setPower(1);

    ASSERT_EQ(busData.dlc, 6);
    EXPECT_THAT(busData.data, ::testing::ElementsAre(0, 0, 0, 10, 1, 1, 0, 0, 0));

    EXPECT_EQ(packet.getLocid(), RR32Can::HumanTurnoutAddress(11));
    EXPECT_EQ(packet.getDirection(), RR32Can::TurnoutDirection::GREEN);
    EXPECT_TRUE(packet.getPower());
  }

  {
    RR32Can::Data busData;
    RR32Can::TurnoutPacket packet(busData);
    packet.initData();
    packet.setLocid(RR32Can::HumanTurnoutAddress(1));
    packet.setDirection(RR32Can::TurnoutDirection::YELLOW);
    packet.setPower(false);

    ASSERT_EQ(busData.dlc, 6);
    EXPECT_THAT(busData.data, ::testing::ElementsAre(0, 0, 0, 0, 2, 0, 0, 0, 0));

    EXPECT_EQ(packet.getLocid(), RR32Can::HumanTurnoutAddress(1));
    EXPECT_EQ(packet.getDirection(), RR32Can::TurnoutDirection::YELLOW);
    EXPECT_FALSE(packet.getPower());
  }
}

TEST(Messages, TurnoutPacket_AddrProto) {
  RR32Can::Data busData;
  RR32Can::TurnoutPacket packet(busData);
  packet.initData();
  packet.setLocid(RR32Can::HumanTurnoutAddress(0x3012));  // MM2 addr
  EXPECT_EQ(packet.getRailProtocol(), RR32Can::RailProtocol::MM1);
  EXPECT_EQ(packet.getLocid().getNumericAddress(), RR32Can::HumanTurnoutAddress(0x12));

  packet.setLocid(RR32Can::HumanTurnoutAddress(0x3812));  // DCC Addr
  EXPECT_EQ(packet.getRailProtocol(), RR32Can::RailProtocol::DCC);
  EXPECT_EQ(packet.getLocid().getNumericAddress(), RR32Can::HumanTurnoutAddress(0x12));

  packet.setLocid(RR32Can::HumanTurnoutAddress(0x2812));  // SX Addr
  EXPECT_EQ(packet.getRailProtocol(), RR32Can::RailProtocol::SX1);
  EXPECT_EQ(packet.getLocid().getNumericAddress(), RR32Can::HumanTurnoutAddress(0x12));
}

TEST(Messages, TurnoutPacket_Serialize) {
  {
    RR32Can::Data data;
    RR32Can::TurnoutPacket packet(data);
    packet.initData();
    RR32Can::MachineTurnoutAddress locid = RR32Can::HumanTurnoutAddress(13);
    locid |= RR32Can::kMMAccessoryAddrStart;
    packet.setLocid(locid);
    packet.setPower(true);
    packet.setDirection(RR32Can::TurnoutDirection::WHITE);

    EXPECT_EQ(data.dlc, 6);
    EXPECT_THAT(data.data, ::testing::ElementsAre(0, 0, 0x30, 12, 3, 1, 0, 0, 0));
  }

  {
    RR32Can::Data data;
    RR32Can::TurnoutPacket packet(data);
    packet.initData();
    RR32Can::MachineTurnoutAddress locid = RR32Can::HumanTurnoutAddress(1);
    locid |= RR32Can::kMMAccessoryAddrStart;
    packet.setLocid(locid);
    packet.setPower(false);
    packet.setDirection(RR32Can::TurnoutDirection::RED);

    EXPECT_EQ(data.dlc, 6);
    EXPECT_THAT(data.data, ::testing::ElementsAre(0, 0, 0x30, 0, 0, 0, 0, 0, 0));
  }

  {
    RR32Can::Data data;
    RR32Can::TurnoutPacket packet(data);
    packet.initData();
    RR32Can::MachineTurnoutAddress locid = RR32Can::HumanTurnoutAddress(320);
    locid |= RR32Can::kMMAccessoryAddrStart;
    packet.setLocid(locid);
    packet.setPower(true);
    packet.setDirection(RR32Can::TurnoutDirection::GREEN);

    EXPECT_EQ(data.dlc, 6);
    EXPECT_THAT(data.data, ::testing::ElementsAre(0, 0, 0x31, 0x3F, 1, 1, 0, 0, 0));
  }
}

TEST(Messages, TurnoutPacket_Deserialize) {
  {
    RR32Can::Data data;
    data.dlc = 6;
    data.data[0] = 0;
    data.data[1] = 0;
    data.data[2] = 0x30;
    data.data[3] = 12;
    data.data[4] = 3;
    data.data[5] = 1;

    RR32Can::TurnoutPacket packet(data);
    RR32Can::MachineTurnoutAddress expectedAddress = RR32Can::kMMAccessoryAddrStart;
    expectedAddress |= RR32Can::HumanTurnoutAddress(13);
    EXPECT_EQ(packet.getLocid(), expectedAddress);
    EXPECT_TRUE(packet.getPower());
    EXPECT_EQ(packet.getDirection(), RR32Can::TurnoutDirection::WHITE);
  }
  {
    RR32Can::Data data;
    data.dlc = 6;
    data.data[0] = 0;
    data.data[1] = 0;
    data.data[2] = 0x30;
    data.data[3] = 0;
    data.data[4] = 0;
    data.data[5] = 0;

    RR32Can::TurnoutPacket packet(data);
    RR32Can::MachineTurnoutAddress expectedAddress = RR32Can::kMMAccessoryAddrStart;
    expectedAddress |= RR32Can::HumanTurnoutAddress(1);
    EXPECT_EQ(packet.getLocid(), expectedAddress);
    EXPECT_FALSE(packet.getPower());
    EXPECT_EQ(packet.getDirection(), RR32Can::TurnoutDirection::RED);
  }
  {
    RR32Can::Data data;
    data.dlc = 6;
    data.data[0] = 0;
    data.data[1] = 0;
    data.data[2] = 0x31;
    data.data[3] = 0x3F;
    data.data[4] = 1;
    data.data[5] = 1;

    const RR32Can::TurnoutPacket packet(data);
    RR32Can::MachineTurnoutAddress expectedAddress = RR32Can::kMMAccessoryAddrStart;
    expectedAddress |= RR32Can::HumanTurnoutAddress(320);
    EXPECT_EQ(packet.getLocid(), expectedAddress);
    EXPECT_TRUE(packet.getPower());
    EXPECT_EQ(packet.getDirection(), RR32Can::TurnoutDirection::GREEN);
  }
}