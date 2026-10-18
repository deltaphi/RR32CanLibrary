#ifndef RR32CAN_TCP_STATION_TEXT_CONFIG_DATA_CALLBACK_H_
#define RR32CAN_TCP_STATION_TEXT_CONFIG_DATA_CALLBACK_H_

#include "RR32Can/messages/Data.h"
#include "RR32Can/callback/ConfigDataCbk.h"

namespace tcp_station {

class TextConfigDataCallback final : public RR32Can::callback::ConfigDataCbk {
 public:
  void startStream() override;
  void addMessage(const RR32Can::Data& data) override;
  void reset() override;
};

}  // namespace tcp_station

#endif  // RR32CAN_TCP_STATION_TEXT_CONFIG_DATA_CALLBACK_H_
