#include <algorithm>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "RR32Can/Station.h"
#include "RR32Can/util/constexpr.h"
#include "RR32CanEngineDb/LokListeConsumer.h"
#include "mocks/Mocks.h"

namespace integration {
class LokListeTest : public ::testing::Test {
 protected:
  void SetUp() override {
    RR32Can::Station::CallbackStruct callbacks;
    callbacks.tx = &tx;
    station.begin(0, callbacks);
    consumer.setStreamEndCallback(&end);
  }

  void start() {
    EXPECT_CALL(tx, SendPacket(RR32Can::util::Request_Config_Data("lokliste", 8)));
    consumer.download(station);
  }

  void feed(const std::string& text, bool corrupt = false, uint8_t headerLength = 7) {
    RR32Can::Crc crc;
    crc.reset();
    for (unsigned char c : text) crc.updateCrc(c);
    auto header = RR32Can::util::Config_Data_Stream(text.size(), crc.getCrc() ^ (corrupt ? 1 : 0));
    header.data.dlc = headerLength;
    station.HandlePacket(header);
    for (size_t offset = 0; offset < text.size(); offset += 8) {
      char chunk[8];
      // Nonzero padding must not reach either the CRC or the text parser.
      memset(chunk, 'X', sizeof(chunk));
      memcpy(chunk, text.data() + offset, std::min(size_t(8), text.size() - offset));
      station.HandlePacket(RR32Can::util::Config_Data_Stream(chunk));
    }
  }

  ::testing::StrictMock<mocks::StationTxCbkMock> tx;
  ::testing::StrictMock<mocks::ConfigDataEndStreamMock> end;
  RR32Can::Station station;
  RR32Can::LokListeConsumer::Entry entries[6];
  RR32Can::LokListeConsumer consumer{entries, 6};
};

TEST_F(LokListeTest, StoresByIndexWithGapsAndMetadata) {
  start();
  EXPECT_CALL(end, streamComplete(&consumer));
  feed("[lokliste]\n .dv=0\n .idx=5\n .llindex=9\n .name=BR 140\n .crc=34783\n"
       " .idx=0\n .name=BR 89\n .idx=2\n .name=\n .future=ignored\n");
  EXPECT_TRUE(consumer.isStreamComplete());
  EXPECT_FALSE(consumer.hasOverflow());
  EXPECT_EQ(consumer.getStoredCount(), 2u);
  EXPECT_EQ(consumer.getNumEnginesKnownByMaster(), 2u);
  EXPECT_STREQ(entries[5].locomotive.getName(), "BR 140");
  EXPECT_EQ(entries[5].llindex, 9u);
  EXPECT_EQ(entries[5].crc, 34783u);
  EXPECT_STREQ(entries[0].locomotive.getName(), "BR 89");
  for (size_t i = 1; i < 5; ++i) EXPECT_TRUE(entries[i].locomotive.isFree());
}

TEST_F(LokListeTest, ReportsOverflowAndStillValidatesEntireStream) {
  start();
  EXPECT_CALL(end, streamComplete(&consumer));
  feed("[lokliste]\n.idx=6\n.name=Outside\n.idx=4294967295\n.name=Huge\n.idx=1\n.name=Inside\n");
  EXPECT_TRUE(consumer.hasOverflow());
  EXPECT_TRUE(consumer.isStreamComplete());
  EXPECT_EQ(consumer.getStoredCount(), 1u);
  EXPECT_EQ(consumer.getNumEnginesKnownByMaster(), 3u);
  EXPECT_STREQ(entries[1].locomotive.getName(), "Inside");
}

TEST_F(LokListeTest, CrcFailureDiscardsPartialResultsIncludingOverflow) {
  start();
  EXPECT_CALL(end, streamAborted(&consumer));
  feed("[lokliste]\n.idx=0\n.name=Inside\n.idx=6\n.name=Outside\n", true);
  EXPECT_FALSE(consumer.isStreamComplete());
  EXPECT_EQ(consumer.getStoredCount(), 0u);
}

TEST_F(LokListeTest, RepeatedDownloadClearsStaleSlotsAndParserState) {
  start();
  EXPECT_CALL(end, streamComplete(&consumer));
  feed("[lokliste]\n.idx=5\n.name=Old\n");
  start();
  EXPECT_TRUE(entries[5].locomotive.isFree());
  EXPECT_CALL(end, streamComplete(&consumer));
  feed("[lokliste]\n.idx=0\n.name=New\n", false, 6);
  EXPECT_EQ(consumer.getStoredCount(), 1u);
  EXPECT_STREQ(entries[0].locomotive.getName(), "New");
}

TEST_F(LokListeTest, RejectsInvalidIndicesWithoutWrapping) {
  start();
  EXPECT_CALL(end, streamAborted(&consumer));
  feed("[lokliste]\n.idx=4294967296\n.name=Invalid\n.idx=-1\n.name=Negative\n");
  EXPECT_FALSE(consumer.isStreamComplete());
  EXPECT_EQ(consumer.getStoredCount(), 0u);
}

TEST_F(LokListeTest, AbortsMalformedTransportAndCanRestart) {
  start();
  EXPECT_CALL(end, streamAborted(&consumer));
  auto bad = RR32Can::util::Config_Data_Stream(8, 0);
  bad.data.dlc = 5;
  station.HandlePacket(bad);
  start();
  EXPECT_CALL(end, streamComplete(&consumer));
  feed("[lokliste]\n.dv=0\n");
  EXPECT_TRUE(consumer.isStreamComplete());
  EXPECT_EQ(consumer.getStoredCount(), 0u);
}

TEST_F(LokListeTest, NullStorageIsSafeAndReportsOverflow) {
  RR32Can::LokListeConsumer empty(nullptr, 10);
  EXPECT_CALL(tx, SendPacket(RR32Can::util::Request_Config_Data("lokliste", 8)));
  empty.download(station);
  feed("[lokliste]\n.idx=0\n.name=Unstored\n");
  EXPECT_TRUE(empty.isStreamComplete());
  EXPECT_TRUE(empty.hasOverflow());
  EXPECT_EQ(empty.capacity(), 0u);
  EXPECT_EQ(empty.getStoredCount(), 0u);
  station.FinishCurrentConfigRequest();
}
}  // namespace integration
