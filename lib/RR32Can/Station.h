#ifndef __RR32CAN__STATION_H__
#define __RR32CAN__STATION_H__

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

#include <RR32Can/Locomotive.h>

#include "RR32Can/Types.h"
#include "RR32Can/callback/AccessoryCbk.h"
#include "RR32Can/callback/ConfigDataCbk.h"
#include "RR32Can/callback/EngineCbk.h"
#include "RR32Can/callback/SystemCbk.h"
#include "RR32Can/callback/TxCbk.h"
#include "RR32Can/messages/CanFrame.h"

#include "RR32Can/messages/SystemMessage.h"

namespace RR32Can {

/*
 * \brief Class Station
 */
class Station {
 public:
  struct CallbackStruct {
    callback::TxCbk* tx = nullptr;
    callback::SystemCbk* system = nullptr;
    callback::EngineCbk* engine = nullptr;
    callback::AccessoryCbk* accessory = nullptr;
    callback::ConfigDataCbk* configData = nullptr;
  };

  /* Initialization & Infrastructure */
  void begin(const uint16_t stationUUID, CallbackStruct& callbacks);
  void loop();

  /* Generic message handling */

  /**
   * \brief handle an incoming packet
   */
  void HandlePacket(const RR32Can::CanFrame& canFrame);

  /**
   * \brief Handle an incoming accessory command
   */
  void HandleAccessoryPacket(const RR32Can::Data& data, bool response);

  void HandleConfigDataStream(const RR32Can::Data& data);

  void HandleSystemCommand(const RR32Can::Identifier& id, const RR32Can::SystemMessage& data);

  /**
   * \brief Construct a Accessory command packet and send via CAN
   */
  void SendAccessoryPacket(RR32Can::MachineTurnoutAddress turnoutAddress, const RailProtocol protocol,
                           const TurnoutDirection direction, const uint8_t power);

  /* Engine Database */
  void FinishCurrentConfigRequest();
  void AbortCurrentConfigRequest();

  /**
   * \brief Sends a request for the information of a specific engine.
   *
   * All incoming results will be directed towards the given engine (i.e., a
   * reference is kept).
   *
   * \param engine An engine with its name set.
   */
  void RequestEngine(const LocomotiveShortInfo& engine, callback::ConfigDataCbk* configDataConsumer);
  void RequestEngineList(const uint8_t offset, callback::ConfigDataCbk* configDataConsumer);

  void RequestEngineDirection(const LocomotiveData& engine);
  void SendEngineDirection(const LocomotiveData& engine, const EngineDirection direction);
  void RequestEngineVelocity(const LocomotiveData& engine);
  void SendEngineVelocity(const LocomotiveData& engine, const LocomotiveData::Velocity_t velocity);
  void RequestEngineFunction(const LocomotiveData& engine, const uint8_t function);
  void RequestEngineAllFunctions(const LocomotiveData& engine);
  void SendEngineFunction(const LocomotiveData& engine, const uint8_t function, const bool value);

  void SendEmergencyStop(const LocomotiveData& engine);
  void SendSystemStop();
  void SendSystemGo();

  void SendPacket(RR32Can::CanFrame& canFrame);

  void HandleLocoDirection(const RR32Can::Data& data);
  void HandleLocoSpeed(const RR32Can::Data& data);
  void HandleLocoFunction(const RR32Can::Data& data);

  void notifyConfigStreamReceived() { FinishCurrentConfigRequest(); }

 private:
  /* Initialization & Infrastructure */
  uint16_t senderHash_ = 0;

  /* Set during begin() */
  CallbackStruct callbacks_;
};

}  // namespace RR32Can

#endif  // __RR32CAN__STATION_H__
