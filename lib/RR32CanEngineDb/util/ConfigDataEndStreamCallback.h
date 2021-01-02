#ifndef __RR32CANENGINEDB__UTIL__CONFIGDATAENDSTREAMCALLBACK_H__
#define __RR32CANENGINEDB__UTIL__CONFIGDATAENDSTREAMCALLBACK_H__

namespace RR32Can {

class ConfigDataConsumer;

/*
 * \brief Class ConfigDataEndStreamCallback
 */
class ConfigDataEndStreamCallback {
 public:
  virtual void streamComplete(ConfigDataConsumer*) = 0;
  virtual void streamAborted(ConfigDataConsumer*) = 0;
};

}  // namespace RR32Can

#endif  // __RR32CANENGINEDB__UTIL__CONFIGDATAENDSTREAMCALLBACK_H__
