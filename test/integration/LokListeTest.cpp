#include <algorithm>
#include <string>

#include "RR32Can/Station.h"
#include "RR32Can/util/constexpr.h"
#include "RR32CanEngineDb/LokListeConsumer.h"
#include "RR32CanEngineDb/util/ConfigDataStreamParser.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/Mocks.h"

namespace integration {
class LokListeTest : public ::testing::Test {
 protected:
  class Callback : public RR32Can::LokListeConsumerCallback {
   public:
    RR32Can::LocomotiveShortInfo entries[6];

    RR32Can::LocomotiveShortInfo* getLokInfoShort(uint32_t index) override {
      return index < 6 ? &entries[index] : nullptr;
    }
  } callback;

  void SetUp() override {
    RR32Can::Station::CallbackStruct callbacks;
    callbacks.tx = &tx;
    station.begin(0, callbacks);
    consumer.setStreamEndCallback(&end);
  }

  void start() {
    EXPECT_CALL(tx, SendPacket(RR32Can::util::Request_Config_Data("lokliste", 8)));
    station.RequestLokListe(&parser);
    parser.startStream(&consumer);
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
  RR32Can::ConfigDataStreamParser parser;
  RR32Can::LokListeConsumer consumer{callback};
};

TEST_F(LokListeTest, StoresByIndexWithGapsAndMetadata) {
  start();
  EXPECT_CALL(end, streamComplete(&consumer));
  feed(
      "[lokliste]\n .dv=0\n .idx=5\n .llindex=9\n .name=BR 140\n .crc=34783\n"
      " .idx=0\n .name=BR 89\n .idx=2\n .name=\n .future=ignored\n");
  EXPECT_TRUE(consumer.isStreamComplete());
  EXPECT_STREQ(callback.entries[5].getName(), "BR 140");
  EXPECT_STREQ(callback.entries[0].getName(), "BR 89");
  for (size_t i = 1; i < 5; ++i) EXPECT_TRUE(callback.entries[i].isFree());
}

TEST_F(LokListeTest, ReportsOverflowAndStillValidatesEntireStream) {
  start();
  EXPECT_CALL(end, streamComplete(&consumer));
  feed("[lokliste]\n.idx=6\n.name=Outside\n.idx=4294967295\n.name=Huge\n.idx=1\n.name=Inside\n");
  EXPECT_TRUE(consumer.isStreamComplete());
  EXPECT_STREQ(callback.entries[1].getName(), "Inside");
}

TEST_F(LokListeTest, CrcFailureDiscardsPartialResultsIncludingOverflow) {
  start();
  EXPECT_CALL(end, streamAborted(&consumer));
  feed("[lokliste]\n.idx=0\n.name=Inside\n.idx=6\n.name=Outside\n", true);
  EXPECT_FALSE(consumer.isStreamComplete());
  // Results are written directly to caller-owned objects; stream abortion
  // does not provide transactional rollback for those objects.
  EXPECT_STREQ(callback.entries[0].getName(), "Inside");
}

TEST_F(LokListeTest, RepeatedDownloadClearsStaleSlotsAndParserState) {
  start();
  EXPECT_CALL(end, streamComplete(&consumer));
  feed("[lokliste]\n.idx=5\n.name=Old\n");
  start();
  EXPECT_CALL(end, streamComplete(&consumer));
  feed("[lokliste]\n.idx=0\n.name=New\n", false, 6);
  EXPECT_STREQ(callback.entries[0].getName(), "New");
}

TEST_F(LokListeTest, RejectsInvalidIndicesWithoutWrapping) {
  start();
  EXPECT_CALL(end, streamAborted(&consumer));
  feed("[lokliste]\n.idx=4294967296\n.name=Invalid\n.idx=-1\n.name=Negative\n");
  EXPECT_FALSE(consumer.isStreamComplete());
  EXPECT_TRUE(callback.entries[0].isFree());
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
}

TEST_F(LokListeTest, CallbackCanSkipAnEngine) {
  start();
  EXPECT_CALL(end, streamComplete(&consumer));
  feed("[lokliste]\n.idx=6\n.name=Skipped\n.idx=1\n.name=Stored\n");
  EXPECT_TRUE(callback.entries[0].isFree());
  EXPECT_STREQ(callback.entries[1].getName(), "Stored");
}
}  // namespace integration
