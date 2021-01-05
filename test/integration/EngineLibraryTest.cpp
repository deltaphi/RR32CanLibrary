#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "RR32Can/Station.h"

#include "RR32CanEngineDb/LocoConsumer.h"
#include "RR32CanEngineDb/LocoListConsumer.h"
#include "RR32CanEngineDb/util/ConfigDataStreamParser.h"

#include "data/ConfigData.h"
#include "mocks/Mocks.h"

namespace integration {

class LocolistTestFixture : public ::testing::Test {
  void SetUp() {
    locoConsumer.setStreamEndCallback(&endStreamCallback);
    locoList.setStreamEndCallback(&endStreamCallback);

    RR32Can::Station::CallbackStruct callbacks;
    callbacks.tx = &txCbk;

    station.begin(0, callbacks);
  }

  void TearDown() {}

 public:
  void RequestEngineListHelper() {
    // Expect the request to be transmitted
    RR32Can::CanFrame expectedFrame1;
    expectedFrame1.id.setCommand(RR32Can::Command::REQUEST_CONFIG_DATA);

    expectedFrame1.data.dlc = 8;
    strncpy(expectedFrame1.data.dataAsString(), "loknamen", 8);

    RR32Can::CanFrame expectedFrame2;
    expectedFrame2.id.setCommand(RR32Can::Command::REQUEST_CONFIG_DATA);
    expectedFrame2.data.dlc = 3;
    strncpy(expectedFrame2.data.dataAsString(), "0 2", 8);

    EXPECT_CALL(txCbk, SendPacket(expectedFrame1));
    EXPECT_CALL(txCbk, SendPacket(expectedFrame2));

    parser.startStream(&locoList);

    // Start request to inject the callback
    station.RequestEngineList(0, &parser);
  }

  void RequestEngineHelper() {
    // Expect the request to be transmitted
    {
      RR32Can::CanFrame frames[3];
      for (auto& frame : frames) {
        frame.id.setCommand(RR32Can::Command::REQUEST_CONFIG_DATA);
      }

      frames[0].data.dlc = 8;
      strncpy(frames[0].data.dataAsString(), "lokinfo", 8);

      frames[1].data.dlc = 8;
      strncpy(frames[1].data.dataAsString(), "98 1001", 8);

      frames[2].data.dlc = 8;

      for (auto& frame : frames) {
        EXPECT_CALL(txCbk, SendPacket(frame));
      }
    }
    parser.startStream(&locoConsumer);

    RR32Can::Locomotive engine;
    engine.setName("98 1001");

    // Start request to inject the callback
    station.RequestEngine(engine, &parser);
  }

  void RequestEngineDataFeeder(RR32Can::Locomotive& destinationLocomotive, const uint16_t streamLength,
                               const uint16_t crc, const char testData[][8]) {
    RR32Can::Identifier id;
    id.setCommand(RR32Can::Command::CONFIG_DATA_STREAM);

    locoConsumer.setEngine(&destinationLocomotive);

    {
      // Send initial packet
      RR32Can::Data data;
      data.dlc = 6;

      data.data[0] = streamLength >> 24;
      data.data[1] = streamLength >> 16;
      data.data[2] = streamLength >> 8;
      data.data[3] = streamLength;

      data.data[4] = crc >> 8;
      data.data[5] = crc;

      station.HandlePacket({id, data});
    }

    for (int i = 0; i < data::testData2NumChunks; ++i) {
      RR32Can::Data data;
      data.dlc = 8;
      strncpy(data.dataAsString(), testData[i], RR32Can::Data::kDataBufferLength);
      station.HandlePacket({id, data});
    }
  }

  ::testing::NaggyMock<mocks::StationTxCbkMock> txCbk;
  RR32Can::Station station;

  RR32Can::ConfigDataStreamParser parser;
  RR32Can::LocoListConsumer locoList;
  RR32Can::LocoConsumer locoConsumer;

  ::testing::StrictMock<mocks::ConfigDataEndStreamMock> endStreamCallback;
};

TEST_F(LocolistTestFixture, Download_Enginelist) {
  RequestEngineListHelper();

  EXPECT_CALL(endStreamCallback, streamComplete(&locoList));

  {
    // Send initial packet
    RR32Can::CanFrame frame;
    frame.id.setCommand(RR32Can::Command::CONFIG_DATA_STREAM);
    frame.data.dlc = 6;

    uint32_t streamLength = data::testData1NumChunks * RR32Can::CanDataMaxLength;

    frame.data.data[0] = streamLength >> 24;
    frame.data.data[1] = streamLength >> 16;
    frame.data.data[2] = streamLength >> 8;
    frame.data.data[3] = streamLength;

    uint16_t expectedCrc = 0x34e1;

    frame.data.data[4] = expectedCrc >> 8;
    frame.data.data[5] = expectedCrc;

    station.HandlePacket(frame);
  }

  for (int i = 0; i < data::testData1NumChunks; ++i) {
    RR32Can::CanFrame frame;
    frame.id.setCommand(RR32Can::Command::CONFIG_DATA_STREAM);
    frame.data.dlc = 8;
    strncpy(frame.data.dataAsString(), data::testData1[i], RR32Can::Data::kDataBufferLength);
    station.HandlePacket(frame);
  }

  EXPECT_EQ(locoList.getNumEnginesKnownByMaster(), 12);
}

TEST_F(LocolistTestFixture, Download_Engine_1) {
  RequestEngineHelper();

  EXPECT_CALL(endStreamCallback, streamComplete(&locoConsumer));

  RR32Can::Locomotive actualEngine;

  EXPECT_NE(actualEngine.getUid(), 0xc00a);
  EXPECT_STRNE(actualEngine.getProtocolString(), "dcc");
  EXPECT_NE(actualEngine.getAddress(), RR32Can::MachineLocomotiveAddress(0xa));

  RequestEngineDataFeeder(actualEngine, data::testData2NumChunks * RR32Can::CanDataMaxLength, 0xdecc, data::testData2);

  EXPECT_EQ(actualEngine.getUid(), 0xc00a);
  EXPECT_STREQ(actualEngine.getProtocolString(), "dcc");
  EXPECT_EQ(actualEngine.getAddress(), RR32Can::MachineLocomotiveAddress(0xa));
}
TEST_F(LocolistTestFixture, Download_Engine_2) {
  RequestEngineHelper();

  EXPECT_CALL(endStreamCallback, streamComplete(&locoConsumer));

  RR32Can::Identifier id;
  id.setCommand(RR32Can::Command::CONFIG_DATA_STREAM);

  RR32Can::Locomotive actualEngine;

  EXPECT_NE(actualEngine.getUid(), 0xc00a);
  EXPECT_STRNE(actualEngine.getProtocolString(), "dcc");
  EXPECT_NE(actualEngine.getAddress(), RR32Can::MachineLocomotiveAddress(0xa));

  RequestEngineDataFeeder(actualEngine, data::testData3NumChunks * RR32Can::CanDataMaxLength, 0x9B2F, data::testData3);

  EXPECT_EQ(actualEngine.getUid(), 0xc032);
  EXPECT_STREQ(actualEngine.getProtocolString(), "dcc");
  EXPECT_EQ(actualEngine.getAddress(), RR32Can::MachineLocomotiveAddress(0x32));
}

}  // namespace integration
