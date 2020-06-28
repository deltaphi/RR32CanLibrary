#ifndef __RR32Can__CONSTANTS_H__
#define __RR32Can__CONSTANTS_H__

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

#include "Types.h"

namespace RR32Can {

// these are the command values as defined by Marklin
// CAN Identifier values are shifted left by one bit.

constexpr uint8_t kSystemCommand = 0x00;
constexpr uint8_t klocoDiscovery = 0x01;
constexpr uint8_t kMfxBind = 0x02;
constexpr uint8_t kMfxVerify = 0x03;
constexpr uint8_t kLocoSpeed = 0x04;
constexpr uint8_t kLocoDirection = 0x05;
constexpr uint8_t kLocoFunction = 0x06;

constexpr uint8_t kReadConfig = 0x07;
constexpr uint8_t kWriteConfig = 0x08;
constexpr uint8_t kAccessorySwitch = 0x0B;
constexpr uint8_t kAccessoryConfig = 0x0C;

constexpr uint8_t kS88Poll = 0x10;
constexpr uint8_t kS88Event = 0x11;
constexpr uint8_t kSX1Event = 0x12;

constexpr uint8_t kPing = 0x18;

constexpr uint8_t kUpdateOffer = 0x19;
constexpr uint8_t kReadConfigData = 0x1A;
constexpr uint8_t kBootloaderCan = 0x1B;
constexpr uint8_t kBootloaderTrack = 0x1C;
constexpr uint8_t kStatusdataConfig = 0x1D;
constexpr uint8_t kRequestConfigData = 0x20;
constexpr uint8_t kConfigDataStream = 0x21;
constexpr uint8_t k60128DataStream = 0x22;

// Subcommands for system commands

constexpr uint8_t kSubcommandSystemStop = 0x00;
constexpr uint8_t kSubcommandSystemGo = 0x01;
constexpr uint8_t kSubcommandSystemHalt = 0x02;
constexpr uint8_t kSubcommandLocoEmergencyStop = 0x03;
constexpr uint8_t kSubcommandLocoStopCycle = 0x04;
constexpr uint8_t kSubcommandLocoDataprotocol = 0x05;
constexpr uint8_t kSubcommandAccessoryTime = 0x06;
constexpr uint8_t kSubcommandMfxFastRead = 0x07;
constexpr uint8_t kSubcommandDataprotocolOn = 0x08;
constexpr uint8_t kSubcommandMfxSetRegistrationCount = 0x09;
constexpr uint8_t kSubcommandSystemOverload = 0x0A;
constexpr uint8_t kSubcommandSystemStatus = 0x0B;
constexpr uint8_t kSubcommandSystemIdentifier = 0x0C;
constexpr uint8_t kSubcommandMfxSeek = 0x30;
constexpr uint8_t kSubcommandSystemReset = 0x80;

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

enum class RailProtocol { MM1, MM2, MFX, SX1, SX2, DCC };

extern const char* kProtocolNameMM1;
extern const char* kProtocolNameMM2;
extern const char* kProtocolNameMFX;
extern const char* kProtocolNameSX1;
extern const char* kProtocolNameSX2;
extern const char* kProtocolNameDCC;
extern const char* kProtocolNameUnknown;

constexpr LocId_t kMMEngineFunctionAddrStart = 0x0000;
constexpr LocId_t kMMEngineFunctionAddrStop = 0x03FF;

constexpr LocId_t kSX1AddrStart = 0x0800;
constexpr LocId_t kSX1AddrStop = 0x0BFF;

constexpr LocId_t kMMFunctionAddrStart = 0x1000;
constexpr LocId_t kMMFunctionAddrStop = 0x13FF;

constexpr LocId_t kMMEngineAddrStart = 0x2000;
constexpr LocId_t kMMEngineAddrStop = 0x23FF;

constexpr LocId_t kSX1AccessoryAddrStart = 0x2800;
constexpr LocId_t kSX1AccessoryAddrStop = 0x2BFF;

constexpr LocId_t kMMAccessoryAddrStart = 0x3000;
constexpr LocId_t kMMAccessoryAddrStop = 0x33FF;

constexpr LocId_t kDCCAccessoryAddrStart = 0x3800;
constexpr LocId_t kDCCccessoryAddrStop = 0x3FFF;

constexpr LocId_t kMFXAddrStart = 0x4000;
constexpr LocId_t kMFXAddrStop = 0x7FFF;

constexpr LocId_t kSX2AddrStart = 0x8000;
constexpr LocId_t kSX2AddrStop = 0xBFFF;

constexpr LocId_t kDCCAddrStart = 0xC000;
constexpr LocId_t kDCCAddrStop = 0xFFFF;

LocId_t getAccessoryLocIdMask(RailProtocol proto);

/// Upper limit for engine speed data
constexpr Velocity_t kMaxEngineVelocity = 1000;

// Filenames of configuration data files
namespace Filenames {

extern const char* kEngine;
extern const char* kEngineResult;
extern const char* kEngineNames;

}  // namespace Filenames

} /* namespace RR32Can */

#endif