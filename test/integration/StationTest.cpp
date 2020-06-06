#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "RR32Can/Station.h"
#include "RR32Can/callback/AccessoryCbk.h"
#include "RR32Can/callback/EngineCbk.h"
#include "RR32Can/callback/SystemCbk.h"
#include "RR32Can/callback/TxCbk.h"

#include "RR32Can/Locomotive.h"

namespace integration {

class SystemCbkMock : public RR32Can::callback::SystemCbk {
 public:
  MOCK_METHOD(void, setSystemState, (bool), (override));
};

class EngineCbkMock : public RR32Can::callback::EngineCbk {
 public:
  MOCK_METHOD(RR32Can::Locomotive *, getLoco, (RR32Can::Locomotive::Uid_t), (override));
  MOCK_METHOD(void, setLocoVelocity, (RR32Can::Locomotive::Uid_t, RR32Can::Velocity_t), (override));
  MOCK_METHOD(void, setLocoVelocity, (RR32Can::Velocity_t), (override));
};

class AccessoryCbkMock : public RR32Can::callback::AccessoryCbk {
 public:
  MOCK_METHOD(void, OnAccessoryPacket, (RR32Can::TurnoutPacket & packet), (override));
};

class StationTxCbkMock : public RR32Can::callback::TxCbk {
 public:
  MOCK_METHOD(void, SendPacket, (const RR32Can::Identifier &, const RR32Can::Data &));
};

class StationTestFixture : public ::testing::Test {
  void SetUp() {
    RR32Can::Station::CallbackStruct callbacks;
    callbacks.tx = &txCbk;

    callbacks.accessory = &accessoryCbk;
    callbacks.system = &systemCbk;
    callbacks.engine = &engineCbk;
    station.begin(0, callbacks);
  }

  void TearDown() {}

 public:
  ::testing::StrictMock<SystemCbkMock> systemCbk;
  ::testing::StrictMock<EngineCbkMock> engineCbk;
  ::testing::StrictMock<AccessoryCbkMock> accessoryCbk;
  ::testing::NaggyMock<StationTxCbkMock> txCbk;
  RR32Can::Station station;
};

TEST_F(StationTestFixture, RecvStop) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command = RR32Can::kSystemCommand;
  data.dlc = 5;
  data.data[4] = RR32Can::kSubcommandSystemStop;

  EXPECT_CALL(systemCbk, setSystemState(false));

  station.HandlePacket(id, data);
}

TEST_F(StationTestFixture, RecvGo) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command = RR32Can::kSystemCommand;
  data.dlc = 5;
  data.data[4] = RR32Can::kSubcommandSystemGo;

  EXPECT_CALL(systemCbk, setSystemState(true));

  station.HandlePacket(id, data);
}

TEST_F(StationTestFixture, SendGo) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command = RR32Can::kSystemCommand;
  data.dlc = 5;
  data.data[4] = RR32Can::kSubcommandSystemGo;

  EXPECT_CALL(txCbk, SendPacket(id, data));

  station.SendSystemGo();
}

TEST_F(StationTestFixture, SendStop) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command = RR32Can::kSystemCommand;
  data.dlc = 5;
  data.data[4] = RR32Can::kSubcommandSystemStop;

  EXPECT_CALL(txCbk, SendPacket(id, data));

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

  EXPECT_CALL(txCbk, SendPacket(id, data));

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

  EXPECT_CALL(accessoryCbk, OnAccessoryPacket(expectedPacket));

  station.HandlePacket(id, data);
}

}  // namespace integration