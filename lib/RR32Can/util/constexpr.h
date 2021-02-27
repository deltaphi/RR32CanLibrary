#ifndef __RR32CAN__UTIL__CONSTEXPR_H__
#define __RR32CAN__UTIL__CONSTEXPR_H__

#include "RR32Can/Constants.h"
#include "RR32Can/Types.h"
#include "RR32Can/messages/CanFrame.h"
#include "RR32Can/messages/S88Event.h"
#include "RR32Can/messages/SystemMessage.h"
#include "RR32Can/messages/TurnoutPacket.h"

#include "RR32Can/Locomotive.h"

namespace RR32Can {
namespace util {

//
// Constexpr create protocol-ful Turnout Addresses
//

constexpr RR32Can::MachineTurnoutAddress MM2(const RR32Can::MachineTurnoutAddress addr) {
  return addr | RR32Can::kMMAccessoryAddrStart;
}

constexpr RR32Can::MachineTurnoutAddress DCC(const RR32Can::MachineTurnoutAddress addr) {
  return addr | RR32Can::kDCCAccessoryAddrStart;
}

constexpr RR32Can::MachineTurnoutAddress SX1(const RR32Can::MachineTurnoutAddress addr) {
  return addr | RR32Can::kSX1AccessoryAddrStart;
}

template <typename T>
constexpr RR32Can::MachineTurnoutAddress MM2_Turnout(T addr) {
  return MM2(RR32Can::MachineTurnoutAddress{addr});
}

template <typename T>
constexpr RR32Can::MachineTurnoutAddress DCC_Turnout(T addr) {
  return DCC(RR32Can::MachineTurnoutAddress{addr});
}

template <typename T>
constexpr RR32Can::MachineTurnoutAddress SX1_Turnout(T addr) {
  return SX1(RR32Can::MachineTurnoutAddress{addr});
}

//
// Constexpr create protocol-ful Engine addresses
//

constexpr RR32Can::MachineLocomotiveAddress MM2(const RR32Can::MachineLocomotiveAddress addr) {
  return addr | RR32Can::kMMEngineAddrStart;
}

constexpr RR32Can::MachineLocomotiveAddress DCC(const RR32Can::MachineLocomotiveAddress addr) {
  return addr | RR32Can::kDCCAddrStart;
}

constexpr RR32Can::MachineLocomotiveAddress SX1(const RR32Can::MachineLocomotiveAddress addr) {
  return addr | RR32Can::kSX1AddrStart;
}

constexpr RR32Can::MachineLocomotiveAddress MFX(const RR32Can::MachineLocomotiveAddress addr) {
  return addr | RR32Can::kMFXAddrStart;
}

template <typename T>
constexpr RR32Can::MachineLocomotiveAddress MM2_Loco(T addr) {
  return MM2(RR32Can::MachineLocomotiveAddress{addr});
}

template <typename T>
constexpr RR32Can::MachineLocomotiveAddress DCC_Loco(T addr) {
  return DCC(RR32Can::MachineLocomotiveAddress{addr});
}

template <typename T>
constexpr RR32Can::MachineLocomotiveAddress SX1_Loco(T addr) {
  return SX1(RR32Can::MachineLocomotiveAddress{addr});
}

template <typename T>
constexpr RR32Can::MachineLocomotiveAddress MFX_Loco(T addr) {
  return MFX(RR32Can::MachineLocomotiveAddress{addr});
}
//
// Create Constexpr CAN frames
//

constexpr RR32Can::CanFrame Turnout(bool response, RR32Can::MachineTurnoutAddress address,
                                    RR32Can::TurnoutDirection direction, bool power) {
  RR32Can::CanFrame frame{{RR32Can::Command::ACCESSORY_SWITCH, 0}, {}};
  frame.id.setResponse(response);
  RR32Can::TurnoutPacket msg{frame.data};
  msg.initData();
  msg.setLocid(address);
  msg.setDirection(direction);
  msg.setPower(power);
  return frame;
}

constexpr RR32Can::CanFrame S88Event(RR32Can::MachineTurnoutAddress device, RR32Can::MachineTurnoutAddress contact,
                                     RR32Can::SensorState oldState, RR32Can::SensorState newState) {
  RR32Can::CanFrame frame{{RR32Can::Command::S88_EVENT, 0}, {}};
  RR32Can::S88Event msg{frame.data};
  msg.initData();
  msg.setSubtype(RR32Can::S88Event::Subtype::RESPONSE);
  msg.setDeviceId(device);
  msg.setContactId(contact);
  msg.setContactId(contact);
  msg.setStates(oldState, newState);
  return frame;
}

constexpr RR32Can::CanFrame S88Event(RR32Can::MachineTurnoutAddress device, RR32Can::MachineTurnoutAddress contact,
                                     RR32Can::SensorState newState) {
  RR32Can::SensorState oldState{newState == RR32Can::SensorState::OPEN ? RR32Can::SensorState::CLOSED
                                                                       : RR32Can::SensorState::OPEN};
  return S88Event(device, contact, oldState, newState);
}

constexpr RR32Can::CanFrame System_Stop(bool response) {
  RR32Can::CanFrame frame{{RR32Can::Command::SYSTEM_COMMAND, 0}, {}};
  frame.id.setResponse(response);
  RR32Can::SystemMessage msg{frame.data};
  msg.initData();
  msg.setSubcommand(RR32Can::SystemSubcommand::SYSTEM_STOP);
  return frame;
}

constexpr RR32Can::CanFrame System_Go(bool response) {
  RR32Can::CanFrame frame{{RR32Can::Command::SYSTEM_COMMAND, 0}, {}};
  frame.id.setResponse(response);
  RR32Can::SystemMessage msg{frame.data};
  msg.initData();
  msg.setSubcommand(RR32Can::SystemSubcommand::SYSTEM_GO);
  return frame;
}

constexpr RR32Can::CanFrame System_Halt(bool response) {
  RR32Can::CanFrame frame{{RR32Can::Command::SYSTEM_COMMAND, 0}, {}};
  frame.id.setResponse(response);
  RR32Can::SystemMessage msg{frame.data};
  msg.initData();
  msg.setSubcommand(RR32Can::SystemSubcommand::SYSTEM_HALT);
  return frame;
}

constexpr RR32Can::CanFrame System_GetStatus() {
  RR32Can::CanFrame frame{{RR32Can::Command::SYSTEM_COMMAND, 0}, {}};
  frame.id.setResponse(false);
  frame.data = Data{};
  frame.data.dlc = 4;
  return frame;
}

constexpr RR32Can::CanFrame System_LocoEmStop(bool response, const RR32Can::MachineLocomotiveAddress address) {
  RR32Can::CanFrame frame{{RR32Can::Command::SYSTEM_COMMAND, 0}, {}};
  frame.id.setResponse(response);
  RR32Can::SystemMessage msg{frame.data};
  msg.initData();
  frame.data.dlc = 5;
  msg.setLocid(address.value());
  msg.setSubcommand(RR32Can::SystemSubcommand::LOCO_EMERGENCY_STOP);
  return frame;
}

constexpr RR32Can::CanFrame LocoSpeed(bool response, const RR32Can::Uid_t uid, const RR32Can::Velocity_t velocity) {
  RR32Can::CanFrame frame{{RR32Can::Command::LOCO_SPEED, 0}, {}};
  frame.id.setResponse(response);
  frame.data.dlc = 6;
  frame.data.setLocid(uid);

  const auto limitedVelocity{(velocity > kMaxEngineVelocity) ? kMaxEngineVelocity : velocity};

  frame.data.data[4] = limitedVelocity >> 8;
  frame.data.data[5] = static_cast<uint8_t>(limitedVelocity);

  return frame;
}

/*
constexpr RR32Can::CanFrame LocoSpeed(bool response, const RR32Can::LocomotiveData & engine) {
  return LocoSpeed(response, engine.getUid(), engine.getVelocity());
}
*/

constexpr RR32Can::CanFrame LocoDirection(bool response, Uid_t uid, EngineDirection direction) {
  RR32Can::CanFrame frame{{RR32Can::Command::LOCO_DIRECTION, 0}, {}};
  frame.id.setResponse(response);
  frame.data.dlc = 5;
  frame.data.setLocid(uid);
  frame.data.data[4] = static_cast<uint8_t>(direction);
  return frame;
}

constexpr RR32Can::CanFrame LocoFunction(bool response, Uid_t uid, uint8_t function, bool onOff) {
  CanFrame frame{{Command::LOCO_FUNCTION, 0}, {}};
  frame.id.setResponse(response);
  frame.data.dlc = 6;
  frame.data.setLocid(uid);
  frame.data.data[4] = function;

  if (onOff) {
    frame.data.data[5] = 1;
  } else {
    frame.data.data[5] = 0;
  }

  return frame;
}

template <typename T>
constexpr uint8_t limitToCANDataLength(T val) {
  return val > RR32Can::CanDataMaxLength ? RR32Can::CanDataMaxLength : val;
}

// Untested so far. Kept around in case it should eventually be needed.
// template<size_t length>
// constexpr RR32Can::CanFrame Config_Data_Stream(const uint8_t data[length]) {
//   static_assert(length <= RR32Can::CanDataMaxLength, "Oversized Data");
//   RR32Can::CanFrame frame{{RR32Can::Command::CONFIG_DATA_STREAM, 0}, {8, *data}};
//   return frame;
// }

constexpr RR32Can::CanFrame Config_Data_Stream(const uint32_t streamLength, const uint16_t crc) {
  RR32Can::CanFrame frame{{RR32Can::Command::CONFIG_DATA_STREAM, 0}, {6, {}}};
  frame.data.data[0] = streamLength >> 24;
  frame.data.data[1] = streamLength >> 16;
  frame.data.data[2] = streamLength >> 8;
  frame.data.data[3] = streamLength;
  frame.data.data[4] = crc >> 8;
  frame.data.data[5] = crc & 0xFF;
  return frame;
}

inline RR32Can::CanFrame Config_Data_Stream(const char data[9]) {
  RR32Can::CanFrame frame{{RR32Can::Command::CONFIG_DATA_STREAM, 0}, {RR32Can::CanDataMaxLength, {}}};
  memcpy(frame.data.dataAsString(), data, RR32Can::CanDataMaxLength);
  return frame;
}

inline RR32Can::CanFrame Request_Config_Data(const char* text, uint8_t msgLen) {
  RR32Can::CanFrame frame{{RR32Can::Command::REQUEST_CONFIG_DATA, 0}, {limitToCANDataLength(msgLen), {}}};
  strncpy(frame.data.dataAsString(), text, RR32Can::Data::kDataBufferLength);
  return frame;
}

inline RR32Can::CanFrame Request_Config_Data(const char* text) {
  return Request_Config_Data(text, static_cast<uint8_t>(strlen(text)));
}

}  // namespace util
}  // namespace RR32Can

#endif  // __RR32CAN__UTIL__CONSTEXPR_H__
