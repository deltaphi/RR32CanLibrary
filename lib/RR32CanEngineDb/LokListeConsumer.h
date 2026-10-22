#ifndef SRC_RR32CAN_LOKLISTECONSUMER_H_
#define SRC_RR32CAN_LOKLISTECONSUMER_H_

#include "RR32CanEngineDb/LoklisteConsumerCallback.h"
#include "RR32CanEngineDb/util/ConfigDataConsumer.h"

namespace RR32Can {

class LoklisteConsumer : public ConfigDataConsumer {
 public:
  // The callback and the objects it returns must outlive the active download.
  explicit LoklisteConsumer(LokListeConsumerCallback& callback) : callback_(callback) {}
  LoklisteConsumer(const LoklisteConsumer&) = delete;
  LoklisteConsumer& operator=(const LoklisteConsumer&) = delete;
  LoklisteConsumer(LoklisteConsumer&&) = delete;
  LoklisteConsumer& operator=(LoklisteConsumer&&) = delete;

  void consumeConfigData(BufferManager& section, BufferManager& key, BufferManager& value) override;
  void setStreamComplete() override;
  void setStreamAborted() override;

  bool isStreamComplete() const { return complete_; }

 private:
  void clear();
  LokListeConsumerCallback& callback_;
  LocomotiveShortInfo* currentLocomotive_ = nullptr;
  bool complete_ = false;
  bool invalid_ = false;
};

}  // namespace RR32Can

#endif  // SRC_RR32CAN_LOKLISTECONSUMER_H_
