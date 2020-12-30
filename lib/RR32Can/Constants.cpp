#include "RR32Can/Constants.h"

namespace RR32Can {

const char* kProtocolNameMM1 = "MM1";
const char* kProtocolNameMM2 = "MM2";
const char* kProtocolNameMFX = "MFX";
const char* kProtocolNameSX1 = "SX1";
const char* kProtocolNameSX2 = "SX2";
const char* kProtocolNameDCC = "DCC";
const char* kProtocolNameUnknown = "UNK";

namespace Filenames {

const char* kEngine = "lokinfo";
const char* kEngineResult = "lokomotive";
const char* kEngineNames = "loknamen";

}  // namespace Filenames

MachineTurnoutAddress getAccessoryLocIdMask(RailProtocol proto) {
  switch (proto) {
    case RailProtocol::MM1:
    case RailProtocol::MM2:
    case RailProtocol::MFX:
      return kMMAccessoryAddrStart;
      break;
    case RailProtocol::SX1:
    case RailProtocol::SX2:
      return kSX1AccessoryAddrStart;
      break;
    case RailProtocol::DCC:
      return kDCCAccessoryAddrStart;
      break;
    default:
      return 0xFF00;  // Guard Value that shows up as an error.
  }
}

MachineLocomotiveAddress getLocomotiveLocIdMask(RailProtocol proto) {
  switch (proto) {
    case RailProtocol::MM1:
    case RailProtocol::MM2:
      return kMMEngineFunctionAddrStart;
      break;
    case RailProtocol::MFX:
      return kMFXAddrStart;
      break;
    case RailProtocol::SX1:
    case RailProtocol::SX2:
      return kSX1AddrStart;
      break;
    case RailProtocol::DCC:
      return kDCCAddrStart;
      break;
    default:
      return 0xFF00;  // Guard Value that shows up as an error.
  }
}

}  // namespace RR32Can
