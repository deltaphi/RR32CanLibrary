#ifndef RR32CAN_UDP_STATION_STRUCTURED_CONFIG_DATA_CALLBACK_H_
#define RR32CAN_UDP_STATION_STRUCTURED_CONFIG_DATA_CALLBACK_H_

#include <cstdio>
#include <memory>
#include <string>

#include "RR32CanEngineDb/util/ConfigDataConsumer.h"

namespace udp_station {

class StructuredConfigDataCallback final : public RR32Can::ConfigDataConsumer {
 public:
  // Writes parsed section/key/value records as TOML [[entries]], preserving order and repeated keys.
  // Values remain strings. An empty filename selects console output. Files must not already exist.
  explicit StructuredConfigDataCallback(const std::string& filename = "");

  void consumeConfigData(RR32Can::BufferManager& section, RR32Can::BufferManager& key,
                         RR32Can::BufferManager& value) override;
  void setStreamComplete() override;
  void setStreamAborted() override;

  void closeFile();

 private:
  void writeText(const std::string& text);

  std::string filename_;
  std::unique_ptr<std::FILE, decltype(&std::fclose)> file_{nullptr, &std::fclose};
};

}  // namespace udp_station

#endif  // RR32CAN_UDP_STATION_STRUCTURED_CONFIG_DATA_CALLBACK_H_
