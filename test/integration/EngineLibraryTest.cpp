#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "RR32Can/Station.h"

#include "RR32Can/util/constexpr.h"
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
    RR32Can::CanFrame expectedFrames[2];
    expectedFrames[0] = RR32Can::util::Request_Config_Data("loknamen");
    expectedFrames[1] = RR32Can::util::Request_Config_Data("0 2");

    EXPECT_CALL(txCbk, SendPacket(expectedFrames[0]));
    EXPECT_CALL(txCbk, SendPacket(expectedFrames[1]));

    parser.startStream(&locoList);

    // Start request to inject the callback
    station.RequestEngineList(0, &parser);
  }

  void RequestEngineHelper() {
    // Expect the request to be transmitted
    {
      RR32Can::CanFrame frames[] = {
          RR32Can::util::Request_Config_Data("lokinfo", 8),
          RR32Can::util::Request_Config_Data("98 1001", 8),
          RR32Can::util::Request_Config_Data("", 8),
      };

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
    locoConsumer.setEngine(&destinationLocomotive);

    {
      // Send initial packet
      auto frame = RR32Can::util::Config_Data_Stream(streamLength, crc);

      station.HandlePacket(frame);
    }

    for (int i = 0; i < data::testData2NumChunks; ++i) {
      auto frame = RR32Can::util::Config_Data_Stream(testData[i]);
      station.HandlePacket(frame);
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
    const uint32_t streamLength = data::testData1NumChunks * RR32Can::CanDataMaxLength;
    const uint16_t expectedCrc = 0x34e1;

    auto frame = RR32Can::util::Config_Data_Stream(streamLength, expectedCrc);
    station.HandlePacket(frame);
  }

  for (int i = 0; i < data::testData1NumChunks; ++i) {
    auto frame = RR32Can::util::Config_Data_Stream(data::testData1[i]);
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
