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
    RR32Can::CanFrame expectedFrames[2];
    for (auto& frame : expectedFrames) {
      frame.id.setCommand(RR32Can::Command::REQUEST_CONFIG_DATA);
    }

    expectedFrames[0].data.dlc = 8;
    strncpy(expectedFrames[0].data.dataAsString(), "loknamen", 8);

    expectedFrames[1].data.dlc = 3;
    strncpy(expectedFrames[1].data.dataAsString(), "0 2", 8);

    EXPECT_CALL(txCbk, SendPacket(expectedFrames[0]));
    EXPECT_CALL(txCbk, SendPacket(expectedFrames[1]));

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

TEST_F(StationTestFixture, RecvStopReq) {
  RR32Can::CanFrame frame;

  frame.id.setCommand(RR32Can::Command::SYSTEM_COMMAND);
  frame.id.setResponse(false);
  frame.data.dlc = 5;
  frame.data.data[4] = RR32Can::kSubcommandSystemStop;

  EXPECT_CALL(systemCbk, setSystemState(false, false));

  station.HandlePacket(frame);
}

TEST_F(StationTestFixture, RecvGoReq) {
  RR32Can::CanFrame frame;

  frame.id.setCommand(RR32Can::Command::SYSTEM_COMMAND);
  frame.id.setResponse(false);
  frame.data.dlc = 5;
  frame.data.data[4] = RR32Can::kSubcommandSystemGo;

  EXPECT_CALL(systemCbk, setSystemState(true, false));

  station.HandlePacket(frame);
}

TEST_F(StationTestFixture, RecvStopResp) {
  RR32Can::CanFrame frame;

  frame.id.setCommand(RR32Can::Command::SYSTEM_COMMAND);
  frame.id.setResponse(true);
  frame.data.dlc = 5;
  frame.data.data[4] = RR32Can::kSubcommandSystemStop;

  EXPECT_CALL(systemCbk, setSystemState(false, true));

  station.HandlePacket(frame);
}

TEST_F(StationTestFixture, RecvEngineEmergencyStop) {
  RR32Can::CanFrame frame;

  frame.id.setCommand(RR32Can::Command::SYSTEM_COMMAND);
  frame.id.setResponse(false);

  frame.data.dlc = 5;
  frame.data.data[4] = RR32Can::kSubcommandLocoEmergencyStop;
  frame.data.data[0] = 0x00;
  frame.data.data[1] = 0x00;
  frame.data.data[2] = 0x48;
  frame.data.data[3] = 0x03;

  EXPECT_CALL(engineCbk, setLocoVelocity(0x4803, 0));

  station.HandlePacket(frame);
}

TEST_F(StationTestFixture, RecvGoResp) {
  RR32Can::CanFrame frame;

  frame.id.setCommand(RR32Can::Command::SYSTEM_COMMAND);
  frame.id.setResponse(true);
  frame.data.dlc = 5;
  frame.data.data[4] = RR32Can::kSubcommandSystemGo;

  EXPECT_CALL(systemCbk, setSystemState(true, true));

  station.HandlePacket(frame);
}

TEST_F(StationTestFixture, SendGo) {
  RR32Can::CanFrame frame;

  frame.id.setCommand(RR32Can::Command::SYSTEM_COMMAND);
  frame.data.dlc = 5;
  frame.data.data[4] = RR32Can::kSubcommandSystemGo;

  EXPECT_CALL(txCbk, SendPacket(frame));

  station.SendSystemGo();
}

TEST_F(StationTestFixture, SendStop) {
  RR32Can::CanFrame frame;

  frame.id.setCommand(RR32Can::Command::SYSTEM_COMMAND);
  frame.data.dlc = 5;
  frame.data.data[4] = RR32Can::kSubcommandSystemStop;

  EXPECT_CALL(txCbk, SendPacket(frame));

  station.SendSystemStop();
}

TEST_F(StationTestFixture, SendAccessory_MFX) {
  RR32Can::CanFrame frame;

  frame.id.setCommand(RR32Can::Command::ACCESSORY_SWITCH);
  frame.data.dlc = 6;
  frame.data.data[0] = 0;
  frame.data.data[1] = 0;
  frame.data.data[2] = 0x30;
  frame.data.data[3] = 0x29;
  frame.data.data[4] = 0x01;
  frame.data.data[5] = 0x01;

  EXPECT_CALL(txCbk, SendPacket(frame));

  station.SendAccessoryPacket(RR32Can::HumanTurnoutAddress(42), RR32Can::RailProtocol::MM2,
                              RR32Can::TurnoutDirection::GREEN, true);
}

TEST_F(StationTestFixture, SendAccessory_DCC) {
  RR32Can::CanFrame frame;

  frame.id.setCommand(RR32Can::Command::ACCESSORY_SWITCH);
  frame.data.dlc = 6;
  frame.data.data[0] = 0;
  frame.data.data[1] = 0;
  frame.data.data[2] = 0x38;
  frame.data.data[3] = 0x29;
  frame.data.data[4] = 0x01;
  frame.data.data[5] = 0x01;

  EXPECT_CALL(txCbk, SendPacket(frame));

  station.SendAccessoryPacket(RR32Can::HumanTurnoutAddress(42), RR32Can::RailProtocol::DCC,
                              RR32Can::TurnoutDirection::GREEN, true);
}

TEST_F(StationTestFixture, ReceiveAccessory_Request) {
  RR32Can::CanFrame frame;

  frame.id.setCommand(RR32Can::Command::ACCESSORY_SWITCH);
  frame.data.dlc = 6;
  frame.data.data[0] = 0;
  frame.data.data[1] = 0;
  frame.data.data[2] = 0x30;  // MM2
  frame.data.data[3] = 0x29;
  frame.data.data[4] = 0x01;
  frame.data.data[5] = 0x01;

  RR32Can::Data expectedData;
  RR32Can::TurnoutPacket expectedPacket(expectedData);
  expectedPacket.initData();
  RR32Can::MachineTurnoutAddress locid = RR32Can::HumanTurnoutAddress(42);
  locid |= RR32Can::kMMAccessoryAddrStart;
  expectedPacket.setLocid(locid);
  expectedPacket.setDirection(RR32Can::TurnoutDirection::GREEN);
  expectedPacket.setPower(true);

  EXPECT_CALL(accessoryCbk, OnAccessoryPacket(expectedPacket, false));

  station.HandlePacket(frame);
}

TEST_F(StationTestFixture, ReceiveAccessory_Response) {
  RR32Can::CanFrame frame;

  frame.id.setCommand(RR32Can::Command::ACCESSORY_SWITCH);
  frame.id.setResponse(true);
  frame.data.dlc = 6;
  frame.data.data[0] = 0;
  frame.data.data[1] = 0;
  frame.data.data[2] = 0x30;  // MM2
  frame.data.data[3] = 0x29;
  frame.data.data[4] = 0x01;
  frame.data.data[5] = 0x01;

  RR32Can::Data expectedData;
  RR32Can::TurnoutPacket expectedPacket(expectedData);
  expectedPacket.initData();
  RR32Can::MachineTurnoutAddress locid = RR32Can::HumanTurnoutAddress(42);
  locid |= RR32Can::kMMAccessoryAddrStart;
  expectedPacket.setLocid(locid);
  expectedPacket.setDirection(RR32Can::TurnoutDirection::GREEN);
  expectedPacket.setPower(true);

  EXPECT_CALL(accessoryCbk, OnAccessoryPacket(expectedPacket, true));

  station.HandlePacket(frame);
}

TEST_F(StationTestFixture, ReceiveConfigData_NoParser_data1) {
  for (int i = 0; i < data::testData1NumChunks; ++i) {
    RR32Can::CanFrame frame;
    frame.id.setCommand(RR32Can::Command::CONFIG_DATA_STREAM);
    frame.data.dlc = 8;
    strncpy(frame.data.dataAsString(), data::testData1[i], RR32Can::Data::kDataBufferLength);
    station.HandlePacket(frame);
  }
}

TEST_F(StationTestFixture, ReceiveConfigData_NoParser_data2) {
  for (int i = 0; i < data::testData2NumChunks; ++i) {
    RR32Can::CanFrame frame;
    frame.id.setCommand(RR32Can::Command::CONFIG_DATA_STREAM);
    frame.data.dlc = 8;
    strncpy(frame.data.dataAsString(), data::testData2[i], RR32Can::Data::kDataBufferLength);
    station.HandlePacket(frame);
  }
}

TEST_F(StationTestFixture, ReceiveConfigData_WithParser_data1) {
  EXPECT_CALL(configDataCbk, startStream_Mock());

  RequestEngineHelper();

  EXPECT_CALL(configDataCbk, addMessage(::testing::_)).Times(data::testData1NumChunks);

  for (int i = 0; i < data::testData1NumChunks; ++i) {
    RR32Can::CanFrame frame;
    frame.id.setCommand(RR32Can::Command::CONFIG_DATA_STREAM);
    frame.data.dlc = 8;
    strncpy(frame.data.dataAsString(), data::testData1[i], RR32Can::Data::kDataBufferLength);
    station.HandlePacket(frame);
  }
}

TEST_F(StationTestFixture, ReceiveConfigData_WithParser_data2) {
  EXPECT_CALL(configDataCbk, startStream_Mock());

  RequestEngineHelper();

  EXPECT_CALL(configDataCbk, addMessage(::testing::_)).Times(data::testData2NumChunks);

  for (int i = 0; i < data::testData2NumChunks; ++i) {
    RR32Can::CanFrame frame;
    frame.id.setCommand(RR32Can::Command::CONFIG_DATA_STREAM);
    frame.data.dlc = 8;
    strncpy(frame.data.dataAsString(), data::testData2[i], RR32Can::Data::kDataBufferLength);
    station.HandlePacket(frame);
  }
}

}  // namespace integration