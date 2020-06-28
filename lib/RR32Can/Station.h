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
#include "RR32Can/messages/Data.h"
#include "RR32Can/messages/Identifier.h"

namespace RR32Can {

/*
 * \brief Class Station
 */
class Station {
 public:
  typedef struct {
    callback::TxCbk* tx = nullptr;
    callback::SystemCbk* system = nullptr;
    callback::EngineCbk* engine = nullptr;
    callback::AccessoryCbk* accessory = nullptr;
    callback::ConfigDataCbk* configData = nullptr;
  } CallbackStruct;

  /* Initialization & Infrastructure */
  void begin(uint16_t stationUUID, CallbackStruct& callbacks);
  void loop();

  /* Generic message handling */

  /**
   * \brief handle an incoming packet
   */
  void HandlePacket(const RR32Can::Identifier& id, const RR32Can::Data& data);

  /**
   * \brief Handle an incoming accessory command
   */
  void HandleAccessoryPacket(const RR32Can::Data& data, bool response);

  void HandleConfigDataStream(const RR32Can::Data& data);

  void HandleSystemCommand(const RR32Can::Data& data);

  /**
   * \brief Construct a Accessory command packet and send via CAN
   */
  void SendAccessoryPacket(RR32Can::MachineTurnoutAddress turnoutAddress, RailProtocol protocol,
                           TurnoutDirection direction, uint8_t power);

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
  void RequestEngine(Locomotive& engine, callback::ConfigDataCbk* configDataConsumer);
  void RequestEngineList(uint8_t offset, callback::ConfigDataCbk* configDataConsumer);

  void RequestEngineDirection(Locomotive& engine);
  void SendEngineDirection(Locomotive& engine, EngineDirection direction);
  void RequestEngineVelocity(Locomotive& engine);
  void SendEngineVelocity(Locomotive& engine, Locomotive::Velocity_t velocity);
  void RequestEngineFunction(Locomotive& engine, uint8_t function);
  void RequestEngineAllFunctions(Locomotive& engine);
  void SendEngineFunction(Locomotive& engine, uint8_t function, bool value);
  void SendEmergencyStop(Locomotive& engine);

  void SendSystemStop();
  void SendSystemGo();

  void HandleLocoDirection(const RR32Can::Data& data);
  void HandleLocoSpeed(const RR32Can::Data& data);
  void HandleLocoFunction(const RR32Can::Data& data);

  void notifyConfigStreamReceived() { FinishCurrentConfigRequest(); }

 private:
  Locomotive* getLocoForData(const RR32Can::Data& data);

  static Locomotive::Uid_t uidFromData(const uint8_t* ptr);
  static void uidToData(uint8_t* ptr, Locomotive::Uid_t uid);

  /* Initialization & Infrastructure */
  uint16_t senderHash;

  /* Set during begin() */
  CallbackStruct callbacks;
};

}  // namespace RR32Can

#endif  // __RR32CAN__STATION_H__
