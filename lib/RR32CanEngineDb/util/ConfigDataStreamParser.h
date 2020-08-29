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
    crc_.reset();
  }

  void startStream(ConfigDataConsumer* consumer) {
    this->consumer_ = consumer;
    textParser_.setConsumer(consumer);
  }

  void addMessage(const RR32Can::Data& data) override;

  void reset() override {
    if (streamState == StreamState::WAITING_FIRST_PACKET || streamState == StreamState::WAITING_DATA_PACKET) {
      if (consumer_ != nullptr) {
        consumer_->setStreamAborted();
      }
    }

    streamState = StreamState::IDLE;
    remainingBytes_ = 0;
    textParser_.reset();
    crc_.reset();
    consumer_ = nullptr;
  }

 private:
  uint32_t remainingBytes_ = 0;
  TextParser textParser_;
  Crc crc_;
  ConfigDataConsumer* consumer_ = nullptr;
};

}  // namespace RR32Can

#endif  // SRC_RR32CAN_CONFIGDATASTREAMPARSER_H_
