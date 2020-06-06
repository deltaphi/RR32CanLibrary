#ifndef __RR32CAN__CALLBACK__CONFIGDATACBK_H__
#define __RR32CAN__CALLBACK__CONFIGDATACBK_H__

namespace RR32Can {
namespace callback {

/*
 * \brief Class ConfigDataCbk
 */
class ConfigDataCbk {
 public:
  enum class StreamState { IDLE = 0, WAITING_FIRST_PACKET, WAITING_DATA_PACKET, STREAM_DONE };

  virtual void startStream() = 0;

  virtual void addMessage(const RR32Can::Data& data) = 0;

  virtual void reset() = 0;

  StreamState getStreamState() const { return streamState; }

  bool isProcessing() const { return streamState != StreamState::IDLE && streamState != StreamState::STREAM_DONE; }

 protected:
  StreamState streamState = StreamState::IDLE;
};

}  // namespace callback
}  // namespace RR32Can

#endif  // __RR32CAN__CALLBACK__CONFIGDATACBK_H__
