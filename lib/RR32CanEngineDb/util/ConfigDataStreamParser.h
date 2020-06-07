#ifndef SRC_RR32CAN_CONFIGDATASTREAMPARSER_H_
#define SRC_RR32CAN_CONFIGDATASTREAMPARSER_H_

#include "RR32Can/messages/Data.h"

#include "RR32Can/callback/ConfigDataCbk.h"
#include "RR32CanEngineDb/util/BufferManager.h"
#include "RR32CanEngineDb/util/ConfigDataConsumer.h"
#include "RR32CanEngineDb/util/Crc.h"
#include "RR32CanEngineDb/util/TextParser.h"

namespace RR32Can {

/*
 * \brief Parser class for a complete ConfigData stream
 */
class ConfigDataStreamParser : public RR32Can::callback::ConfigDataCbk {
 public:
  void startStream() override {
    streamState = StreamState::WAITING_FIRST_PACKET;
    crc.reset();
  }

  void startStream(ConfigDataConsumer* consumer) {
    this->consumer = consumer;
    textParser.setConsumer(consumer);
  }

  void addMessage(const RR32Can::Data& data) override;

  void reset() override {
    if (streamState == StreamState::WAITING_FIRST_PACKET || streamState == StreamState::WAITING_DATA_PACKET) {
      if (consumer != nullptr) {
        consumer->setStreamAborted();
      }
    }

    streamState = StreamState::IDLE;
    remainingBytes = 0;
    textParser.reset();
    crc.reset();
    consumer = nullptr;
  }

 private:
  uint32_t remainingBytes = 0;
  TextParser textParser;
  Crc crc;
  ConfigDataConsumer* consumer = nullptr;
};

}  // namespace RR32Can

#endif  // SRC_RR32CAN_CONFIGDATASTREAMPARSER_H_
