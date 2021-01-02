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
    RR32Can::Identifier expectedIdentifier;
    expectedIdentifier.setCommand(RR32Can::Command::REQUEST_CONFIG_DATA);

    RR32Can::Data expectedData1;
    expectedData1.dlc = 8;
    strncpy(expectedData1.dataAsString(), "loknamen", 8);

    RR32Can::Data expectedData2;
    expectedData2.dlc = 3;
    strncpy(expectedData2.dataAsString(), "0 2", 8);

    EXPECT_CALL(txCbk, SendPacket(expectedIdentifier, expectedData1));
    EXPECT_CALL(txCbk, SendPacket(expectedIdentifier, expectedData2));

    parser.startStream(&locoList);

    // Start request to inject the callback
    station.RequestEngineList(0, &parser);
  }

  void RequestEngineHelper() {
    // Expect the request to be transmitted
    RR32Can::Identifier id;
    id.setCommand(RR32Can::Command::REQUEST_CONFIG_DATA);

    RR32Can::Data data[3];
    data[0].dlc = 8;
    strncpy(data[0].dataAsString(), "lokinfo", 8);

    data[1].dlc = 8;
    strncpy(data[1].dataAsString(), "98 1001", 8);

    data[2].dlc = 8;

    EXPECT_CALL(txCbk, SendPacket(id, data[0]));
    EXPECT_CALL(txCbk, SendPacket(id, data[1]));
    EXPECT_CALL(txCbk, SendPacket(id, data[2]));

    parser.startStream(&locoConsumer);

    RR32Can::Locomotive engine;
    engine.setName("98 1001");

    // Start request to inject the callback
    station.RequestEngine(engine, &parser);
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

  RR32Can::Identifier id;
  id.setCommand(RR32Can::Command::CONFIG_DATA_STREAM);

  {
    // Send initial packet
    RR32Can::Data data;
    data.dlc = 6;

    uint32_t streamLength = data::testData1NumChunks * RR32Can::CanDataMaxLength;

    data.data[0] = streamLength >> 24;
    data.data[1] = streamLength >> 16;
    data.data[2] = streamLength >> 8;
    data.data[3] = streamLength;

    uint16_t expectedCrc = 0x34e1;

    data.data[4] = expectedCrc >> 8;
    data.data[5] = expectedCrc;

    station.HandlePacket(id, data);
  }

  for (int i = 0; i < data::testData1NumChunks; ++i) {
    RR32Can::Data data;
    data.dlc = 8;
    strncpy(data.dataAsString(), data::testData1[i], RR32Can::Data::kDataBufferLength);
    station.HandlePacket(id, data);
  }

  EXPECT_EQ(locoList.getNumEnginesKnownByMaster(), 12);
}

TEST_F(LocolistTestFixture, Download_Engine) {
  RequestEngineHelper();

  EXPECT_CALL(endStreamCallback, streamComplete(&locoConsumer));

  RR32Can::Identifier id;
  id.setCommand(RR32Can::Command::CONFIG_DATA_STREAM);

  RR32Can::Locomotive actualEngine;
  locoConsumer.setEngine(&actualEngine);

  EXPECT_NE(actualEngine.getUid(), 0xc00a);
  EXPECT_STRNE(actualEngine.getProtocolString(), "dcc");
  EXPECT_NE(actualEngine.getAddress(), RR32Can::MachineLocomotiveAddress(0xa));

  {
    // Send initial packet
    RR32Can::Data data;
    data.dlc = 6;

    uint32_t streamLength = data::testData2NumChunks * RR32Can::CanDataMaxLength;

    data.data[0] = streamLength >> 24;
    data.data[1] = streamLength >> 16;
    data.data[2] = streamLength >> 8;
    data.data[3] = streamLength;

    uint16_t expectedCrc = 0xdecc;

    data.data[4] = expectedCrc >> 8;
    data.data[5] = expectedCrc;

    station.HandlePacket(id, data);
  }

  for (int i = 0; i < data::testData2NumChunks; ++i) {
    RR32Can::Data data;
    data.dlc = 8;
    strncpy(data.dataAsString(), data::testData2[i], RR32Can::Data::kDataBufferLength);
    station.HandlePacket(id, data);
  }

  EXPECT_EQ(actualEngine.getUid(), 0xc00a);
  EXPECT_STREQ(actualEngine.getProtocolString(), "dcc");
  EXPECT_EQ(actualEngine.getAddress(), RR32Can::MachineLocomotiveAddress(0xa));
}

}  // namespace integration
