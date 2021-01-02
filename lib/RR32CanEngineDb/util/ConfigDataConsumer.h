#ifndef __RR32CAN__CONFIGDATACONSUMER_H__
#define __RR32CAN__CONFIGDATACONSUMER_H__

#include "RR32Can/messages/Data.h"
#include "RR32CanEngineDb/util/ConfigDataEndStreamCallback.h"
#include "RR32CanEngineDb/util/TextParser.h"

namespace RR32Can {

/*
 * \brief Interface for receiving elements of a config data stream
 */
class ConfigDataConsumer : public TextParserConsumer {
 public:
  virtual ~ConfigDataConsumer() = default;
  virtual void setStreamComplete() = 0;
  virtual void setStreamAborted() = 0;
  void setStreamEndCallback(ConfigDataEndStreamCallback* callback) { streamEndCallback_ = callback; }

 protected:
  ConfigDataEndStreamCallback* streamEndCallback_;
};

}  // namespace RR32Can

#endif  // __RR32CAN__CONFIGDATACONSUMER_H__
