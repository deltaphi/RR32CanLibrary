#ifndef RR32CAN_UDP_STATION_TEXT_CONFIG_DATA_CALLBACK_H_
#define RR32CAN_UDP_STATION_TEXT_CONFIG_DATA_CALLBACK_H_

#include <cstdio>
#include <memory>
#include <string>

#include "RR32Can/callback/ConfigDataCbk.h"
#include "RR32Can/messages/Data.h"

namespace udp_station {

class TextConfigDataCallback final : public RR32Can::callback::ConfigDataCbk {
 public:
  enum class FileMode { PreserveNullCharacters, OmitNullCharacters };

  // An empty filename selects console output. Files must not already exist.
  explicit TextConfigDataCallback(const std::string& filename = "",
                                  FileMode fileMode = FileMode::PreserveNullCharacters);

  void startStream() override;
  void addMessage(const RR32Can::Data& data) override;
  void reset() override;

  void closeFile();

 private:
  std::string filename_;
  FileMode fileMode_ = FileMode::PreserveNullCharacters;
  std::unique_ptr<std::FILE, decltype(&std::fclose)> file_{nullptr, &std::fclose};
};

}  // namespace udp_station

#endif  // RR32CAN_UDP_STATION_TEXT_CONFIG_DATA_CALLBACK_H_
