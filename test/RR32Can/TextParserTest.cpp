#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <functional>
#include <memory>

#include "RR32Can/util/TextParser.h"
#include "data/ConfigData.h"

namespace RR32Can {

class CallbackMock : public RR32Can::TextParserConsumer {
 public:
  MOCK_METHOD3(mocked_method, void(const char* section, const char* key, const char* value));

  void consumeConfigData(RR32Can::BufferManager& section, RR32Can::BufferManager& key, RR32Can::BufferManager& value) {
    mocked_method(section.data(), key.data(), value.data());
  }
};

class TextParserFixture : public ::testing::Test {
 protected:
  std::unique_ptr<::testing::StrictMock<CallbackMock>> mock;
  RR32Can::TextParser parser;

  void reportParseError() { parser.reportParseError(); }

  void SetUp() {
    mock = std::make_unique<::testing::StrictMock<CallbackMock>>();
    parser.setConsumer(mock.get());
    EXPECT_EQ(RR32Can::TextParser::State::LOOKING_FOR_KEY_OR_SECTION_START, parser.getState());
  }
};

TEST_F(TextParserFixture, reportError) {
  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("ERROR"), ::testing::StrEq("ERROR"), ::testing::StrEq("ERROR")));
  reportParseError();
  // Parser is reset to initial state
  EXPECT_EQ(RR32Can::TextParser::State::ERROR, parser.getState());
}

TEST_F(TextParserFixture, oversized_data) {
  char buffer[] = "This data is muuuuuuch too long to fit into the input buffer.";

  RR32Can::BufferManager::size_type length = static_cast<RR32Can::BufferManager::size_type>(strlen(buffer));
  RR32Can::BufferManager mgr(buffer, length, length);

  EXPECT_LT(RR32Can::TextParser::kBufferLength, mgr.length());

  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("ERROR"), ::testing::StrEq("ERROR"), ::testing::StrEq("ERROR")));

  parser.addText(mgr);

  // Parser is reset to initial state
  EXPECT_EQ(RR32Can::TextParser::State::ERROR, parser.getState());
}

TEST_F(TextParserFixture, oversized_data_exact) {
  char buffer[] = "This data is muuuuuuch too lon";

  RR32Can::BufferManager::size_type length = static_cast<RR32Can::BufferManager::size_type>(strlen(buffer));
  RR32Can::BufferManager mgr(buffer, length, length);

  EXPECT_EQ(RR32Can::TextParser::kBufferLength, mgr.length());

  // No tokens found in the input

  parser.addText(mgr);

  // Parser remains in initial state
  EXPECT_EQ(RR32Can::TextParser::State::LOOKING_FOR_KEY_OR_SECTION_START, parser.getState());
}

TEST_F(TextParserFixture, oversized_data_plusone) {
  char buffer[] = "This data is muuuuuuch too long";

  RR32Can::BufferManager::size_type length = static_cast<RR32Can::BufferManager::size_type>(strlen(buffer));
  RR32Can::BufferManager mgr(buffer, length, length);

  EXPECT_EQ(RR32Can::TextParser::kBufferLength + 1, mgr.length());

  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("ERROR"), ::testing::StrEq("ERROR"), ::testing::StrEq("ERROR")));

  parser.addText(mgr);

  // Parser is reset to initial state
  EXPECT_EQ(RR32Can::TextParser::State::ERROR, parser.getState());
}

TEST_F(TextParserFixture, FindToken_01) {
  parser.reset();

  char* buffer = data::testData1[0];
  RR32Can::BufferManager::size_type length = static_cast<RR32Can::BufferManager::size_type>(strlen(buffer));
  RR32Can::BufferManager mgr(buffer, length, length);

  parser.buffer.push_back(mgr);
  RR32Can::TextParser::FindTokenResult result = parser.findToken(0, RR32Can::TextParser::kKeyOrSectionStart, nullptr);

  EXPECT_TRUE(result.success);
  EXPECT_EQ(1, result.consumed);
  EXPECT_TRUE(parser.section.empty());
  EXPECT_TRUE(parser.key.empty());
  EXPECT_TRUE(parser.value.empty());

  parser.reset();

  parser.buffer.push_back(mgr);
  result = parser.findToken(1, RR32Can::TextParser::kSectionStop, &parser.section);

  EXPECT_FALSE(result.success);
  EXPECT_EQ(8, result.consumed);
  EXPECT_FALSE(parser.section.empty());
  EXPECT_TRUE(parser.key.empty());
  EXPECT_TRUE(parser.value.empty());

  parser.buffer.erase();

  buffer = data::testData1[1];
  length = static_cast<RR32Can::BufferManager::size_type>(strlen(buffer));
  mgr = RR32Can::BufferManager(buffer, length, length);

  parser.buffer.push_back(mgr);
  result = parser.findToken(0, RR32Can::TextParser::kSectionStop, &parser.section);

  EXPECT_TRUE(result.success);
  EXPECT_EQ(2, result.consumed);
  EXPECT_FALSE(parser.section.empty());
  EXPECT_STREQ("loknamen", parser.section.data());
  EXPECT_TRUE(parser.key.empty());
  EXPECT_TRUE(parser.value.empty());
}

TEST_F(TextParserFixture, testData1) {
  EXPECT_CALL(*mock,
              mocked_method(::testing::StrEq("loknamen"), ::testing::StrEq("wert"), ::testing::StrEq("BR 96 1234")));

  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("loknamen"), ::testing::StrEq("wert"), ::testing::StrEq("ET 515")));

  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("numloks"), ::testing::StrEq("wert"), ::testing::StrEq("12")));

  for (uint8_t i = 0; i < data::testData1NumChunks; ++i) {
    char* buffer = data::testData1[i];
    RR32Can::BufferManager::size_type length = static_cast<RR32Can::BufferManager::size_type>(strlen(buffer));
    RR32Can::BufferManager mgr(buffer, length, length);
    if (i < data::testData1NumChunks - 1) {
      EXPECT_EQ(8, mgr.length());
    } else {
      EXPECT_EQ(2, mgr.length());
    }

    parser.addText(mgr);
  }

  // Parser is reset to initial state
  EXPECT_EQ(RR32Can::TextParser::State::LOOKING_FOR_KEY_OR_SECTION_START, parser.getState());
}

TEST_F(TextParserFixture, testData2) {
  EXPECT_CALL(*mock,
              mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq("uid"), ::testing::StrEq("0xc00a")));

  EXPECT_CALL(*mock,
              mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq("name"), ::testing::StrEq("98 1001")));

  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq("typ"), ::testing::StrEq("dcc")));

  EXPECT_CALL(*mock,
              mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq("adresse"), ::testing::StrEq("0xa")));

  EXPECT_CALL(*mock,
              mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq("mfxuid"), ::testing::StrEq("0x1")));

  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq("symbol"), ::testing::StrEq("22")))
      .Times(2);

  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq("av"), ::testing::StrEq("60")));

  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq("bv"), ::testing::StrEq("40")));

  EXPECT_CALL(*mock,
              mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq("volume"), ::testing::StrEq("100")));

  EXPECT_CALL(*mock,
              mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq("velocity"), ::testing::StrEq("0")));

  EXPECT_CALL(*mock,
              mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq("richtung"), ::testing::StrEq("0")));

  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq("vmax"), ::testing::StrEq("255")));

  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq("vmin"), ::testing::StrEq("13")));

  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq("fkt"), ::testing::StrEq("")))
      .Times(16);

  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq(".wert"), ::testing::StrEq("0")))
      .Times(16);

  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq(".typ"), ::testing::StrEq("4")));
  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq(".typ"), ::testing::StrEq("51")));
  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq(".typ"), ::testing::StrEq("52")));
  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq(".typ"), ::testing::StrEq("53")));
  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq(".typ"), ::testing::StrEq("8")));
  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq(".typ"), ::testing::StrEq("0")))
      .Times(11);

  EXPECT_CALL(*mock, mocked_method(::testing::StrEq("lokomotive"), ::testing::StrEq(".dauer"), ::testing::StrEq("0")))
      .Times(16);

  for (uint8_t i = 0; i < data::testData2NumChunks; ++i) {
    char* buffer = data::testData2[i];
    RR32Can::BufferManager::size_type length = 8;
    RR32Can::BufferManager mgr(buffer, length, length);
    EXPECT_EQ(8, mgr.length());

    parser.addText(mgr);
    std::string myString(buffer, 0, 8);
    std::cout << myString;
  }
  std::cout << std::endl;
  // Parser is reset to initial state
  EXPECT_EQ(RR32Can::TextParser::State::LOOKING_FOR_KEY_OR_SECTION_START, parser.getState());
}

} /* namespace RR32Can */