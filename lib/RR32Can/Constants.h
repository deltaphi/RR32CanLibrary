#ifndef __RR32Can__CONSTANTS_H__
#define __RR32Can__CONSTANTS_H__

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

#include "RR32Can/Types.h"

namespace RR32Can {

// these are the command values as defined by Marklin
// CAN Identifier values are shifted left by one bit.

constexpr CommandByte_t kSystemCommand = 0x00;
constexpr CommandByte_t klocoDiscovery = 0x01;
constexpr CommandByte_t kMfxBind = 0x02;
constexpr CommandByte_t kMfxVerify = 0x03;
constexpr CommandByte_t kLocoSpeed = 0x04;
constexpr CommandByte_t kLocoDirection = 0x05;
constexpr CommandByte_t kLocoFunction = 0x06;

constexpr CommandByte_t kReadConfig = 0x07;
constexpr CommandByte_t kWriteConfig = 0x08;
constexpr CommandByte_t kAccessorySwitch = 0x0B;
constexpr CommandByte_t kAccessoryConfig = 0x0C;

constexpr CommandByte_t kS88Poll = 0x10;
constexpr CommandByte_t kS88Event = 0x11;
constexpr CommandByte_t kSX1Event = 0x12;

constexpr CommandByte_t kPing = 0x18;

constexpr CommandByte_t kUpdateOffer = 0x19;
constexpr CommandByte_t kReadConfigData = 0x1A;
constexpr CommandByte_t kBootloaderCan = 0x1B;
constexpr CommandByte_t kBootloaderTrack = 0x1C;
constexpr CommandByte_t kStatusdataConfig = 0x1D;
constexpr CommandByte_t kRequestConfigData = 0x20;
constexpr CommandByte_t kConfigDataStream = 0x21;
constexpr CommandByte_t k60128DataStream = 0x22;

enum class Command : CommandByte_t {
  SYSTEM_COMMAND = kSystemCommand,
  LOCO_DISCOVERY = klocoDiscovery,
  MFX_BIND = kMfxBind,
  MFX_VERIFY = kMfxVerify,
  LOCO_SPEED = kLocoSpeed,
  LOCO_DIRECTION = kLocoDirection,
  LOCO_FUNCTION = kLocoFunction,
  READ_CONFIG = kReadConfig,
  WRITE_CONFIG = kWriteConfig,
  ACCESSORY_SWITCH = kAccessorySwitch,
  ACCESSORY_CONFIG = kAccessoryConfig,
  S88_POLL = kS88Poll,
  S88_EVENT = kS88Event,
  SX1_EVENT = kSX1Event,
  PING = kPing,
  UPDATE_OFFER = kUpdateOffer,
  READ_CONFIG_DATA = kReadConfigData,
  BOOTLOADER_CAN = kBootloaderCan,
  BOOTLOADER_TRACK = kBootloaderTrack,
  STATUSDATA_CONFIG = kStatusdataConfig,
  REQUEST_CONFIG_DATA = kRequestConfigData,
  CONFIG_DATA_STREAM = kConfigDataStream,
  CMD_60128_DATA_STREAM = k60128DataStream
};

// Subcommands for system commands

constexpr CommandByte_t kSubcommandSystemStop = 0x00;
constexpr CommandByte_t kSubcommandSystemGo = 0x01;
constexpr CommandByte_t kSubcommandSystemHalt = 0x02;
constexpr CommandByte_t kSubcommandLocoEmergencyStop = 0x03;
constexpr CommandByte_t kSubcommandLocoStopCycle = 0x04;
constexpr CommandByte_t kSubcommandLocoDataprotocol = 0x05;
constexpr CommandByte_t kSubcommandAccessoryTime = 0x06;
constexpr CommandByte_t kSubcommandMfxFastRead = 0x07;
constexpr CommandByte_t kSubcommandDataprotocolOn = 0x08;
constexpr CommandByte_t kSubcommandMfxSetRegistrationCount = 0x09;
constexpr CommandByte_t kSubcommandSystemOverload = 0x0A;
constexpr CommandByte_t kSubcommandSystemStatus = 0x0B;
constexpr CommandByte_t kSubcommandSystemIdentifier = 0x0C;
constexpr CommandByte_t kSubcommandMfxSeek = 0x30;
constexpr CommandByte_t kSubcommandSystemReset = 0x80;

enum class SystemSubcommand : CommandByte_t {
  SYSTEM_STOP = kSubcommandSystemStop,
  SYSTEM_GO = kSubcommandSystemGo,
  SYSTEM_HALT = kSubcommandSystemHalt,
  LOCO_EMERGENCY_STOP = kSubcommandLocoEmergencyStop,
  LOCO_STOP_CYCLE = kSubcommandLocoStopCycle,
  LOCO_DATAPROTOCOL = kSubcommandLocoDataprotocol,
  ACCESSORY_TIME = kSubcommandAccessoryTime,
  MFX_FAST_READ = kSubcommandMfxFastRead,
  DATAPROTOCOL_ON = kSubcommandDataprotocolOn,
  MFX_SET_REGISTRATION_COUNT = kSubcommandMfxSetRegistrationCount,
  SYSTEM_OVERLOAD = kSubcommandSystemOverload,
  SYSTEM_STATUS = kSubcommandSystemStatus,
  SYSTEM_IDENTIFIER = kSubcommandSystemIdentifier,
  MFX_SEEK = kSubcommandMfxSeek,
  SYSTEM_RESET = kSubcommandSystemReset
};

/// Maximum CAN payload length
constexpr const uint8_t CanDataMaxLength = 8;

/// Maximum length of an Engine name. TODO: Is this value sufficient?
constexpr const uint8_t kEngineNameLength = 32;

/// Base UUID for enthusiast devices
constexpr const uint32_t enthusiastStationBaseUUID = 0x00001800;

/// Maximum number of engines stored concurrently
constexpr const uint8_t kMaxNumEnginesKnown = 10;

/// Number of engine names to request for download at once
constexpr const uint8_t kNumEngineNamesDownload = 2;

/**
 * \brief Number of engines stored in the engine browser.
 *
 * Should be greater or equal than kNumEngineNamesDownload.
 */
constexpr const uint8_t kEngineBrowserEntries = kNumEngineNamesDownload;

static_assert(kEngineBrowserEntries >= kNumEngineNamesDownload, "Cannot store all downloaded Engine enries");

// Turnout Constants
constexpr const MachineTurnoutAddress kTurnoutAddressMin = 0;
constexpr const MachineTurnoutAddress kTurnoutAddressMax = 319;

// Address ranges for engine protocols

extern const char* kProtocolNameMM1;
extern const char* kProtocolNameMM2;
extern const char* kProtocolNameMFX;
extern const char* kProtocolNameSX1;
extern const char* kProtocolNameSX2;
extern const char* kProtocolNameDCC;
extern const char* kProtocolNameUnknown;

constexpr MachineLocomotiveAddress kMMEngineFunctionAddrStart{0x0000};
constexpr MachineLocomotiveAddress kMMEngineFunctionAddrStop{0x03FF};

constexpr MachineLocomotiveAddress kSX1AddrStart{0x0800};
constexpr MachineLocomotiveAddress kSX1AddrStop{0x0BFF};

constexpr MachineLocomotiveAddress kMMFunctionAddrStart{0x1000};
constexpr MachineLocomotiveAddress kMMFunctionAddrStop{0x13FF};

constexpr MachineLocomotiveAddress kMMEngineAddrStart{0x2000};
constexpr MachineLocomotiveAddress kMMEngineAddrStop{0x23FF};

constexpr MachineTurnoutAddress kSX1AccessoryAddrStart{0x2800};
constexpr MachineTurnoutAddress kSX1AccessoryAddrStop{0x2BFF};

constexpr MachineTurnoutAddress kMMAccessoryAddrStart{0x3000};
constexpr MachineTurnoutAddress kMMAccessoryAddrStop{0x33FF};

constexpr MachineTurnoutAddress kDCCAccessoryAddrStart{0x3800};
constexpr MachineTurnoutAddress kDCCccessoryAddrStop{0x3FFF};

constexpr MachineLocomotiveAddress kMFXAddrStart{0x4000};
constexpr MachineLocomotiveAddress kMFXAddrStop{0x7FFF};

constexpr MachineLocomotiveAddress kSX2AddrStart{0x8000};
constexpr MachineLocomotiveAddress kSX2AddrStop{0xBFFF};

constexpr MachineLocomotiveAddress kDCCAddrStart{0xC000};
constexpr MachineLocomotiveAddress kDCCAddrStop{0xFFFF};

/// Upper limit for engine speed data
constexpr Velocity_t kMaxEngineVelocity{1000};

// Filenames of configuration data files
namespace Filenames {

extern const char* kEngine;
extern const char* kEngineResult;
extern const char* kEngineNames;

}  // namespace Filenames

} /* namespace RR32Can */

#endif