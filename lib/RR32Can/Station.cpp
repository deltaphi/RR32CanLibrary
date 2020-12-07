#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

#include "RR32Can/StlAdapter.h"

#include "RR32Can/Station.h"

#include "RR32Can/Constants.h"
#include "RR32Can/messages/Data.h"
#include "RR32Can/messages/Identifier.h"
#include "RR32Can/messages/TurnoutPacket.h"
#include "RR32Can/util/utils.h"

#include "RR32Can_config.h"

namespace RR32Can {

void Station::begin(uint16_t stationUUID, CallbackStruct& callbacks) {
  AbortCurrentConfigRequest();
  this->callbacks_ = callbacks;
  senderHash_ = computeSenderHash(stationUUID);
}

void Station::loop() {}

void Station::FinishCurrentConfigRequest() {
  if (callbacks_.configData != nullptr) {
    callbacks_.configData->reset();
    callbacks_.configData = nullptr;
  }
}

void Station::AbortCurrentConfigRequest() { FinishCurrentConfigRequest(); }

void Station::HandleConfigDataStream(const RR32Can::Data& data) {
  if (callbacks_.configData != nullptr && callbacks_.configData->isProcessing()) {
#if (LOG_CONFIG_DATA_STREAM_LEVEL >= LOG_CONFIG_DATA_STREAM_LEVEL_ALL)
    data.printAsHex();
    printf(" '");
    data.printAsText();
    printf("' ");
#endif
    callbacks_.configData->addMessage(data);

  } else {
    // Unknown message or no message expected. Ignore.
    printf("Ignoring ConfigData Stream: ");
    data.printAsText();
  }
}

void Station::HandleSystemCommand(const RR32Can::Data& data) {
  printf("System Command. ");
  if (data.dlc >= 5) {
    printf(" Subcommand: ");
    switch (data.data[4]) {
      case RR32Can::kSubcommandSystemGo:
        printf("GO!\n");
        if (callbacks_.system != nullptr) {
          callbacks_.system->setSystemState(true);
        }
        break;
      case RR32Can::kSubcommandSystemHalt: {
        printf("Halt!\n");
        if (callbacks_.engine != nullptr) {
          Locomotive::Uid_t uid = uidFromData(data.data);
          callbacks_.engine->setLocoVelocity(uid, 0);
        }
      } break;
      case RR32Can::kSubcommandSystemStop:
        printf("STOP!\n");
        if (callbacks_.system != nullptr) {
          callbacks_.system->setSystemState(false);
        }
        break;
      case kSubcommandLocoEmergencyStop: {
        Locomotive::Uid_t uid = uidFromData(data.data);
        if (callbacks_.engine != nullptr) {
          callbacks_.engine->setLocoVelocity(uid, 0);
        }
      } break;
      case RR32Can::kSubcommandSystemIdentifier:
        printf("Identifier.\n");
        break;
      case RR32Can::kSubcommandSystemOverload:
        printf("OVERLOAD!\n");
        break;
      case RR32Can::kSubcommandSystemReset:
        printf("Reset.\n");
        // MaerklinSystem.systemOn = false; // TODO: Bring back the System
        // class
        break;
      case RR32Can::kSubcommandSystemStatus:
        printf("Status.\n");
        break;

      default:
        printf("unknown.\n");
        break;
    }

  } else {
    // Not a valid command.
    printf("Invalid command.\n");
  }
}

void Station::RequestEngineList(uint8_t offset, callback::ConfigDataCbk* configDataConsumer) {
  AbortCurrentConfigRequest();
  callbacks_.configData = configDataConsumer;

  Identifier id{kRequestConfigData, senderHash_};

  /* First packet */
  Data data1;
  data1.dlc = 8;
  strncpy(data1.dataAsString(), RR32Can::Filenames::kEngineNames, Data::kDataBufferLength);

  /* Second packet */
  Data data2;
  data2.dlc = snprintf(data2.dataAsString(), Data::kDataBufferLength, "%d %d", offset, kNumEngineNamesDownload);
  if (data2.dlc <= CanDataMaxLength) {
    // expectedConfigData = ConfigDataStreamType::LOKNAMEN;
    if (callbacks_.configData != nullptr) {
      callbacks_.configData->startStream();
    }
    callbacks_.tx->SendPacket(id, data1);
    callbacks_.tx->SendPacket(id, data2);
  } else {
    // Packet was oversized. Abort the operation.
    return;
  }
}

void Station::RequestEngine(Locomotive& engine, callback::ConfigDataCbk* configDataConsumer) {
  if (!engine.isNameKnown()) {
#if LOG_CAN_OUT_MSG == STD_ON
    printf("Station::RequestEngine: No Engine Name given, dropping request.\n");
#endif
    return;
  }

  if (callbacks_.configData != nullptr && callbacks_.configData->isProcessing()) {
    /* Given an empty engine slot or a request is already in progress. Abort. */
#if LOG_CAN_OUT_MSG == STD_ON
    printf("Station::RequestEngine: Request in progress, dropping request.\n");
#endif
    return;
  }

  AbortCurrentConfigRequest();

  callbacks_.configData = configDataConsumer;
  if (callbacks_.configData != nullptr) {
    callbacks_.configData->startStream();
  }

  Identifier id{kRequestConfigData, senderHash_};
  Data data;

  /* First packet */
  data.dlc = 8;
  strncpy(data.dataAsString(), RR32Can::Filenames::kEngine, Data::kDataBufferLength);
  callbacks_.tx->SendPacket(id, data);

  /* Second packet */
  data.reset();
  data.dlc = 8;
  const char* engineName = engine.getName();
  uint8_t engineNameLength = strlen(engineName);
  strncpy(data.dataAsString(), engineName, CanDataMaxLength);

  callbacks_.tx->SendPacket(id, data);

  /* Third packet */
  data.reset();
  data.dlc = 8;
  if (engineNameLength > CanDataMaxLength) {
    strncpy(data.dataAsString(), engineName + CanDataMaxLength, CanDataMaxLength);
  }

  callbacks_.tx->SendPacket(id, data);
}

void Station::uidToData(uint8_t* ptr, Locomotive::Uid_t uid) {
  ptr[0] = static_cast<uint8_t>(uid >> 24);
  ptr[1] = static_cast<uint8_t>(uid >> 16);
  ptr[2] = static_cast<uint8_t>(uid >> 8);
  ptr[3] = static_cast<uint8_t>(uid);
}

void Station::RequestEngineDirection(Locomotive& engine) {
  RR32Can::Identifier identifier{kLocoDirection, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 4;
  uidToData(data.data, engine.getUid());

  callbacks_.tx->SendPacket(identifier, data);
}

void Station::SendEngineDirection(Locomotive& engine, EngineDirection direction) {
  RR32Can::Identifier identifier{kLocoDirection, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 5;
  uidToData(data.data, engine.getUid());

  if ((direction == EngineDirection::FORWARD) || (direction == EngineDirection::REVERSE) ||
      (direction == EngineDirection::CHANGE_DIRECTION)) {
    data.data[4] = static_cast<uint8_t>(direction);
    callbacks_.tx->SendPacket(identifier, data);
  }  // else: not implemented.
}

void Station::RequestEngineVelocity(Locomotive& engine) {
  RR32Can::Identifier identifier{kLocoSpeed, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 4;
  uidToData(data.data, engine.getUid());

  callbacks_.tx->SendPacket(identifier, data);
}

void Station::SendEngineVelocity(Locomotive& engine, Locomotive::Velocity_t velocity) {
  RR32Can::Identifier identifier{kLocoSpeed, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 6;
  uidToData(data.data, engine.getUid());

  if (velocity > kMaxEngineVelocity) {
    velocity = kMaxEngineVelocity;
  }
  data.data[4] = velocity >> 8;
  data.data[5] = velocity;

  callbacks_.tx->SendPacket(identifier, data);
}

void Station::RequestEngineFunction(Locomotive& engine, uint8_t function) {
  RR32Can::Identifier identifier{kLocoFunction, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 5;
  uidToData(data.data, engine.getUid());
  data.data[4] = function;

  callbacks_.tx->SendPacket(identifier, data);
}

void Station::RequestEngineAllFunctions(Locomotive& engine) {
  RR32Can::Identifier identifier{kLocoFunction, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 5;
  uidToData(data.data, engine.getUid());
  for (uint8_t i = 0; i < 16; ++i) {
    data.data[4] = i;
    callbacks_.tx->SendPacket(identifier, data);
  }
}

void Station::SendEngineFunction(Locomotive& engine, uint8_t function, bool value) {
  RR32Can::Identifier identifier{kLocoFunction, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 6;
  uidToData(data.data, engine.getUid());
  data.data[4] = function;

  if (value) {
    data.data[5] = 1;
  } else {
    data.data[5] = 0;
  }

  callbacks_.tx->SendPacket(identifier, data);
}

void Station::SendEmergencyStop(Locomotive& engine) {
  RR32Can::Identifier identifier{kSystemCommand, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 5;
  uidToData(data.data, engine.getUid());
  data.data[4] = kSubcommandLocoEmergencyStop;
  callbacks_.tx->SendPacket(identifier, data);
}

void Station::SendSystemStop() {
  RR32Can::Identifier identifier{kSystemCommand, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 5;
  data.data[4] = kSubcommandSystemStop;
  callbacks_.tx->SendPacket(identifier, data);
}

void Station::SendSystemGo() {
  RR32Can::Identifier identifier{kSystemCommand, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 5;
  data.data[4] = kSubcommandSystemGo;
  callbacks_.tx->SendPacket(identifier, data);
}

void Station::SendAccessoryPacket(RR32Can::MachineTurnoutAddress turnoutAddress, RailProtocol protocol,
                                  TurnoutDirection direction, uint8_t power) {
  RR32Can::Identifier identifier{kAccessorySwitch, this->senderHash_};

  RR32Can::TurnoutPacket payload;
  payload.locid = turnoutAddress;  // Set the turnout address
  payload.locid = getAccessoryLocIdMask(protocol) | payload.locid.value();
  payload.position = direction;  // Set the turnout direction
  payload.power = power;

  // Serialize the CAN packet and send it
  RR32Can::Data data;
  payload.serialize(data);

#if (LOG_CAN_OUT_MSG == STD_ON)
  printf("Setting turnout %d to position %s %s\n", payload.locid & (~0x3000), payload.position == 0 ? "RED " : "GREEN",
         payload.power ? "(ON) " : "(OFF)");
#endif

  callbacks_.tx->SendPacket(identifier, data);
}

void Station::HandlePacket(const RR32Can::Identifier& id, const RR32Can::Data& data) {
#if LOG_CAN_RAW_MSG_IN == STD_ON
  id.printAll();
  printf("\n");
#endif

  switch (id.command_) {
    case RR32Can::kSystemCommand:
      this->HandleSystemCommand(data);
      break;

    case RR32Can::kPing:
#if (LOG_PING == STD_ON)
      printf("Ping. Payload: 0x");
      data.printAsHex();
      printf("\n");
#endif
      break;

    case RR32Can::kAccessorySwitch:
      printf("Accessory Switch. Details: ");
      this->HandleAccessoryPacket(data, id.response_);
      printf("\n");
      break;

    case RR32Can::kLocoDirection:
      this->HandleLocoDirection(data);
      break;

    case RR32Can::kLocoSpeed:
      this->HandleLocoSpeed(data);
      break;

    case RR32Can::kLocoFunction:
      this->HandleLocoFunction(data);
      break;

    case RR32Can::kRequestConfigData:
#if (LOG_CONFIG_DATA_STREAM_LEVEL >= LOG_CONFIG_DATA_STREAM_LEVEL_ALL)
      printf("Request Config Data. Payload: ");
      data.printAsText();
      printf("\n");
#endif
      break;

    case RR32Can::kConfigDataStream:
#if (LOG_CONFIG_DATA_STREAM_LEVEL >= LOG_CONFIG_DATA_STREAM_LEVEL_ALL)
      printf("Config Data Stream.\n");
#endif
      this->HandleConfigDataStream(data);
      break;

    default:
      printf("Unknown or not implemented. Dump: 0x%#02x\n", id.command_);
      data.printAsHex();
      printf("\n");
      break;
  }
}

void Station::HandleAccessoryPacket(const RR32Can::Data& data, bool request) {
  RR32Can::TurnoutPacket turnoutPacket = RR32Can::TurnoutPacket::FromCanPacket(data);
  turnoutPacket.printAll();

  if (callbacks_.accessory != nullptr) {
    callbacks_.accessory->OnAccessoryPacket(turnoutPacket, request);
  }
}

Locomotive::Uid_t Station::uidFromData(const uint8_t* ptr) {
  return (static_cast<Locomotive::Uid_t>(ptr[0]) << 24) | (static_cast<Locomotive::Uid_t>(ptr[1]) << 16) |
         (static_cast<Locomotive::Uid_t>(ptr[2]) << 8) | (ptr[3]);
}

Locomotive* Station::getLocoForData(const RR32Can::Data& data) {
  Locomotive::Uid_t uid = uidFromData(data.data);
  if (callbacks_.engine != nullptr) {
    return callbacks_.engine->getLoco(uid);
  } else {
    return nullptr;
  }
}

void Station::HandleLocoDirection(const RR32Can::Data& data) {
  if (data.dlc == 5) {
    // response.
    Locomotive* engine = getLocoForData(data);
    if (engine == nullptr) {
      return;
    }

    EngineDirection direction = static_cast<EngineDirection>(data.data[4]);
    if (direction == EngineDirection::FORWARD || direction == EngineDirection::REVERSE) {
      engine->setDirection(direction);
    } else if (direction == EngineDirection::CHANGE_DIRECTION) {
      engine->changeDirection();
    }
  }  // else: requests are ignored.
}

void Station::HandleLocoSpeed(const RR32Can::Data& data) {
  if (data.dlc == 6) {
    Locomotive::Uid_t uid = uidFromData(data.data);
    Locomotive::Velocity_t velocity = (data.data[4] << 8) | data.data[5];
    if (callbacks_.engine != nullptr) {
      callbacks_.engine->setLocoVelocity(uid, velocity);
    }
  }
}

void Station::HandleLocoFunction(const RR32Can::Data& data) {
  if (data.dlc == 6) {
    Locomotive* engine = getLocoForData(data);
    if (engine == nullptr) {
      return;
    }
    engine->setFunction(data.data[4], data.data[5]);
  }
}

}  // namespace RR32Can
