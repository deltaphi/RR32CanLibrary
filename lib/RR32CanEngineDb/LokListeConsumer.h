#ifndef SRC_RR32CAN_LOKLISTECONSUMER_H_
#define SRC_RR32CAN_LOKLISTECONSUMER_H_

#include <cstddef>
#include "RR32Can/Locomotive.h"
#include "RR32CanEngineDb/util/ConfigDataConsumer.h"
#include "RR32CanEngineDb/util/ConfigDataStreamParser.h"

namespace RR32Can {

class Station;

class LokListeConsumer : public ConfigDataConsumer {
 public:
  struct Entry {
    LocomotiveShortInfo locomotive;
    uint32_t llindex = 0;
    uint32_t crc = 0;  // Opaque per-entry metadata, not the stream CRC.
  };

  // Storage and this consumer must outlive the active download. No allocation is performed.
  LokListeConsumer(Entry* storage, size_t capacity);
  LokListeConsumer(const LokListeConsumer&) = delete;
  LokListeConsumer& operator=(const LokListeConsumer&) = delete;
  LokListeConsumer(LokListeConsumer&&) = delete;
  LokListeConsumer& operator=(LokListeConsumer&&) = delete;

  void download(Station& station);
  void consumeConfigData(BufferManager& section, BufferManager& key, BufferManager& value) override;
  void setStreamComplete() override;
  void setStreamAborted() override;

  bool isStreamComplete() const { return complete_; }
  bool hasOverflow() const { return overflow_; }
  size_t getStoredCount() const;
  size_t getNumEnginesKnownByMaster() const { return occupiedCount_; }
  size_t capacity() const { return capacity_; }

 private:
  void clear();
  void finishEntry();
  Entry* storage_;
  size_t capacity_;
  ConfigDataStreamParser parser_;
  Entry pending_;
  uint32_t index_ = 0;
  size_t occupiedCount_ = 0;
  bool haveIndex_ = false;
  bool complete_ = false;
  bool overflow_ = false;
  bool invalid_ = false;
};

}  // namespace RR32Can

#endif  // SRC_RR32CAN_LOKLISTECONSUMER_H_
