#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "RR32Can/Station.h"

#include "RR32Can/Locomotive.h"

#include "data/ConfigData.h"

#include "mocks/Mocks.h"

namespace integration {

class StationTestFixture : public ::testing::Test {
  void SetUp() {
    RR32Can::Station::CallbackStruct callbacks;
    callbacks.tx = &txCbk;

    callbacks.accessory = &accessoryCbk;
    callbacks.system = &systemCbk;
    callbacks.engine = &engineCbk;
    // callbacks.configData = &configDataCbk;

    station.begin(0, callbacks);
  }

  void TearDown() {}

 public:
  void RequestEngineHelper() {
    // Expect the request to be transmitted
    RR32Can::Identifier expectedIdentifier;
    expectedIdentifier.command_ = RR32Can::kRequestConfigData;

    RR32Can::Data expectedData1;
    expectedData1.dlc = 8;
    strncpy(expectedData1.dataAsString(), "loknamen", 8);

    RR32Can::Data expectedData2;
    expectedData2.dlc = 3;
    strncpy(expectedData2.dataAsString(), "0 2", 8);

    EXPECT_CALL(txCbk, SendPacket(expectedIdentifier, expectedData1));
    EXPECT_CALL(txCbk, SendPacket(expectedIdentifier, expectedData2));

    // Start request to inject the callback
    station.RequestEngineList(0, &configDataCbk);
  }

  ::testing::StrictMock<mocks::SystemCbkMock> systemCbk;
  ::testing::StrictMock<mocks::EngineCbkMock> engineCbk;
  ::testing::StrictMock<mocks::AccessoryCbkMock> accessoryCbk;
  ::testing::StrictMock<mocks::ConfigDataCbkMock> configDataCbk;
  ::testing::NaggyMock<mocks::StationTxCbkMock> txCbk;
  RR32Can::Station station;
};

TEST_F(StationTestFixture, RecvStop) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command_ = RR32Can::kSystemCommand;
  data.dlc = 5;
  data.data[4] = RR32Can::kSubcommandSystemStop;

  EXPECT_CALL(systemCbk, setSystemState(false));

  station.HandlePacket(id, data);
}

TEST_F(StationTestFixture, RecvGo) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command_ = RR32Can::kSystemCommand;
  data.dlc = 5;
  data.data[4] = RR32Can::kSubcommandSystemGo;

  EXPECT_CALL(systemCbk, setSystemState(true));

  station.HandlePacket(id, data);
}

TEST_F(StationTestFixture, SendGo) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command_ = RR32Can::kSystemCommand;
  data.dlc = 5;
  data.data[4] = RR32Can::kSubcommandSystemGo;

  EXPECT_CALL(txCbk, SendPacket(id, data));

  station.SendSystemGo();
}

TEST_F(StationTestFixture, SendStop) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command_ = RR32Can::kSystemCommand;
  data.dlc = 5;
  data.data[4] = RR32Can::kSubcommandSystemStop;

  EXPECT_CALL(txCbk, SendPacket(id, data));

  station.SendSystemStop();
}

TEST_F(StationTestFixture, SendAccessory_MFX) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command_ = RR32Can::kAccessorySwitch;
  data.dlc = 6;
  data.data[0] = 0;
  data.data[1] = 0;
  data.data[2] = 0x30;
  data.data[3] = 0x29;
  data.data[4] = 0x01;
  data.data[5] = 0x01;

  EXPECT_CALL(txCbk, SendPacket(id, data));

  station.SendAccessoryPacket(RR32Can::HumanTurnoutAddress(42), RR32Can::RailProtocol::MM2,
                              RR32Can::TurnoutDirection::GREEN, true);
}

TEST_F(StationTestFixture, SendAccessory_DCC) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command_ = RR32Can::kAccessorySwitch;
  data.dlc = 6;
  data.data[0] = 0;
  data.data[1] = 0;
  data.data[2] = 0x38;
  data.data[3] = 0x29;
  data.data[4] = 0x01;
  data.data[5] = 0x01;

  EXPECT_CALL(txCbk, SendPacket(id, data));

  station.SendAccessoryPacket(RR32Can::HumanTurnoutAddress(42), RR32Can::RailProtocol::DCC,
                              RR32Can::TurnoutDirection::GREEN, true);
}

TEST_F(StationTestFixture, ReceiveAccessory_Request) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command_ = RR32Can::kAccessorySwitch;
  id.response = false;
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

  EXPECT_CALL(accessoryCbk, OnAccessoryPacket(expectedPacket, false));

  station.HandlePacket(id, data);
}

TEST_F(StationTestFixture, ReceiveAccessory_Response) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command_ = RR32Can::kAccessorySwitch;
  id.response = true;
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

  EXPECT_CALL(accessoryCbk, OnAccessoryPacket(expectedPacket, true));

  station.HandlePacket(id, data);
}

TEST_F(StationTestFixture, ReceiveConfigData_NoParser_data1) {
  RR32Can::Identifier id;
  id.command_ = RR32Can::kConfigDataStream;

  for (int i = 0; i < data::testData1NumChunks; ++i) {
    RR32Can::Data data;
    data.dlc = 8;
    strncpy(data.dataAsString(), data::testData1[i], RR32Can::Data::kDataBufferLength);
    station.HandlePacket(id, data);
  }
}

TEST_F(StationTestFixture, ReceiveConfigData_NoParser_data2) {
  RR32Can::Identifier id;
  id.command_ = RR32Can::kConfigDataStream;

  for (int i = 0; i < data::testData2NumChunks; ++i) {
    RR32Can::Data data;
    data.dlc = 8;
    strncpy(data.dataAsString(), data::testData2[i], RR32Can::Data::kDataBufferLength);
    station.HandlePacket(id, data);
  }
}

TEST_F(StationTestFixture, ReceiveConfigData_WithParser_data1) {
  EXPECT_CALL(configDataCbk, startStream_Mock());

  RequestEngineHelper();

  EXPECT_CALL(configDataCbk, addMessage(::testing::_)).Times(data::testData1NumChunks);

  RR32Can::Identifier id;
  id.command_ = RR32Can::kConfigDataStream;

  for (int i = 0; i < data::testData1NumChunks; ++i) {
    RR32Can::Data data;
    data.dlc = 8;
    strncpy(data.dataAsString(), data::testData1[i], RR32Can::Data::kDataBufferLength);
    station.HandlePacket(id, data);
  }
}

TEST_F(StationTestFixture, ReceiveConfigData_WithParser_data2) {
  EXPECT_CALL(configDataCbk, startStream_Mock());

  RequestEngineHelper();

  EXPECT_CALL(configDataCbk, addMessage(::testing::_)).Times(data::testData2NumChunks);

  RR32Can::Identifier id;
  id.command_ = RR32Can::kConfigDataStream;

  for (int i = 0; i < data::testData2NumChunks; ++i) {
    RR32Can::Data data;
    data.dlc = 8;
    strncpy(data.dataAsString(), data::testData2[i], RR32Can::Data::kDataBufferLength);
    station.HandlePacket(id, data);
  }
}

}  // namespace integration