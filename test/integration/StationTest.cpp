#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "RR32Can/Station.h"
#include "RR32Can/StationCbk.h"
#include "RR32Can/StationTxCbk.h"

#include "RR32Can/Locomotive.h"

namespace integration {

class StationCbkMock : public RR32Can::StationCbk {
 public:
  MOCK_METHOD(RR32Can::Locomotive *, getLoco, (RR32Can::Locomotive::Uid_t), (override));
  MOCK_METHOD(void, setLocoVelocity, (RR32Can::Locomotive::Uid_t, RR32Can::Velocity_t), (override));
  MOCK_METHOD(void, setLocoVelocity, (RR32Can::Velocity_t), (override));
  MOCK_METHOD(void, setSystemState, (bool), (override));
  MOCK_METHOD(void, OnAccessoryPacket, (RR32Can::TurnoutPacket & packet), (override));
};

class StationTxCbkMock : public RR32Can::StationTxCbk {
 public:
  MOCK_METHOD(void, SendPacket, (const RR32Can::Identifier &, const RR32Can::Data &));
};

class StationTestFixture : public ::testing::Test {
  void SetUp() { station.begin(0, callback, txCallback); }

  void TearDown() {}

 public:
  ::testing::StrictMock<StationCbkMock> callback;
  ::testing::NaggyMock<StationTxCbkMock> txCallback;
  RR32Can::Station station;
};

TEST_F(StationTestFixture, RecvStop) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command = RR32Can::kSystemCommand;
  data.dlc = 5;
  data.data[4] = RR32Can::kSubcommandSystemStop;

  EXPECT_CALL(callback, setSystemState(false));

  station.HandlePacket(id, data);
}

TEST_F(StationTestFixture, RecvGo) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command = RR32Can::kSystemCommand;
  data.dlc = 5;
  data.data[4] = RR32Can::kSubcommandSystemGo;

  EXPECT_CALL(callback, setSystemState(true));

  station.HandlePacket(id, data);
}

TEST_F(StationTestFixture, SendGo) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command = RR32Can::kSystemCommand;
  data.dlc = 5;
  data.data[4] = RR32Can::kSubcommandSystemGo;

  EXPECT_CALL(txCallback, SendPacket(id, data));

  station.SendSystemGo();
}

TEST_F(StationTestFixture, SendStop) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command = RR32Can::kSystemCommand;
  data.dlc = 5;
  data.data[4] = RR32Can::kSubcommandSystemStop;

  EXPECT_CALL(txCallback, SendPacket(id, data));

  station.SendSystemStop();
}

TEST_F(StationTestFixture, SendAccessory) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command = RR32Can::kAccessorySwitch;
  data.dlc = 6;
  data.data[0] = 0;
  data.data[1] = 0;
  data.data[2] = 0x30;
  data.data[3] = 0x29;
  data.data[4] = 0x01;
  data.data[5] = 0x01;

  EXPECT_CALL(txCallback, SendPacket(id, data));

  station.SendAccessoryPacket(RR32Can::HumanTurnoutAddress(42), RR32Can::TurnoutDirection::GREEN, true);
}

TEST_F(StationTestFixture, ReceiveAccessory) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command = RR32Can::kAccessorySwitch;
  data.dlc = 6;
  data.data[0] = 0;
  data.data[1] = 0;
  data.data[2] = 0x30;  // MM2
  data.data[3] = 0x29;
  data.data[4] = 0x01;
  data.data[5] = 0x01;

  RR32Can::TurnoutPacket expectedPacket;
  expectedPacket.locid = 0x3000 /* MM2 */ | RR32Can::MachineTurnoutAddress(RR32Can::HumanTurnoutAddress(42)).value();
  expectedPacket.position =
      static_cast<std::underlying_type_t<RR32Can::TurnoutDirection>>(RR32Can::TurnoutDirection::GREEN);
  expectedPacket.power = true;

  EXPECT_CALL(callback, OnAccessoryPacket(expectedPacket));

  station.HandlePacket(id, data);
}

}  // namespace integration