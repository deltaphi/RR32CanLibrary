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

void Station::HandleSystemCommand(const RR32Can::Identifier& id, const RR32Can::SystemMessage& msg) {
  printf("System Command. ");
  if (msg.length() >= 5) {
    printf(" Subcommand: ");
    switch (msg.getSubcommand()) {
      case RR32Can::SystemSubcommand::SYSTEM_GO:
        printf("GO!\n");
        if (callbacks_.system != nullptr) {
          callbacks_.system->setSystemState(true, id.isResponse());
        }
        break;
      case RR32Can::SystemSubcommand::SYSTEM_HALT: {
        printf("Halt!\n");
        if (callbacks_.engine != nullptr) {
          Locomotive::Uid_t uid = msg.getUid();
          callbacks_.engine->setLocoVelocity(uid, 0);
        }
      } break;
      case RR32Can::SystemSubcommand::SYSTEM_STOP:
        printf("STOP!\n");
        if (callbacks_.system != nullptr) {
          callbacks_.system->setSystemState(false, id.isResponse());
        }
        break;
      case SystemSubcommand::LOCO_EMERGENCY_STOP: {
        Locomotive::Uid_t uid = msg.getUid();
        if (callbacks_.engine != nullptr) {
          callbacks_.engine->setLocoVelocity(uid, 0);
        }
      } break;
      case RR32Can::SystemSubcommand::SYSTEM_IDENTIFIER:
        printf("Identifier.\n");
        break;
      case RR32Can::SystemSubcommand::SYSTEM_OVERLOAD:
        printf("OVERLOAD!\n");
        break;
      case RR32Can::SystemSubcommand::SYSTEM_RESET:
        printf("Reset.\n");
        // MaerklinSystem.systemOn = false; // TODO: Bring back the System
        // class
        break;
      case RR32Can::SystemSubcommand::SYSTEM_STATUS:
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

  Identifier id{Command::REQUEST_CONFIG_DATA, senderHash_};

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

  Identifier id{Command::REQUEST_CONFIG_DATA, senderHash_};
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

void Station::RequestEngineDirection(Locomotive& engine) {
  RR32Can::Identifier identifier{Command::LOCO_DIRECTION, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 4;
  data.setLocid(engine.getUid());

  callbacks_.tx->SendPacket(identifier, data);
}

void Station::SendEngineDirection(Locomotive& engine, EngineDirection direction) {
  RR32Can::Identifier identifier{Command::LOCO_DIRECTION, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 5;
  data.setLocid(engine.getUid());

  if ((direction == EngineDirection::FORWARD) || (direction == EngineDirection::REVERSE) ||
      (direction == EngineDirection::CHANGE_DIRECTION)) {
    data.data[4] = static_cast<uint8_t>(direction);
    callbacks_.tx->SendPacket(identifier, data);
  }  // else: not implemented.
}

void Station::RequestEngineVelocity(Locomotive& engine) {
  RR32Can::Identifier identifier{Command::LOCO_SPEED, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 4;
  data.setLocid(engine.getUid());

  callbacks_.tx->SendPacket(identifier, data);
}

void Station::SendEngineVelocity(Locomotive& engine, Locomotive::Velocity_t velocity) {
  RR32Can::Identifier identifier{Command::LOCO_SPEED, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 6;
  data.setLocid(engine.getUid());

  if (velocity > kMaxEngineVelocity) {
    velocity = kMaxEngineVelocity;
  }
  data.data[4] = velocity >> 8;
  data.data[5] = velocity;

  callbacks_.tx->SendPacket(identifier, data);
}

void Station::RequestEngineFunction(Locomotive& engine, uint8_t function) {
  RR32Can::Identifier identifier{Command::LOCO_FUNCTION, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 5;
  data.setLocid(engine.getUid());
  data.data[4] = function;

  callbacks_.tx->SendPacket(identifier, data);
}

void Station::RequestEngineAllFunctions(Locomotive& engine) {
  RR32Can::Identifier identifier{Command::LOCO_FUNCTION, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 5;
  data.setLocid(engine.getUid());
  for (uint8_t i = 0; i < 16; ++i) {
    data.data[4] = i;
    callbacks_.tx->SendPacket(identifier, data);
  }
}

void Station::SendEngineFunction(Locomotive& engine, uint8_t function, bool value) {
  RR32Can::Identifier identifier{Command::LOCO_FUNCTION, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 6;
  data.setLocid(engine.getUid());
  data.data[4] = function;

  if (value) {
    data.data[5] = 1;
  } else {
    data.data[5] = 0;
  }

  callbacks_.tx->SendPacket(identifier, data);
}

void Station::SendEmergencyStop(Locomotive& engine) {
  RR32Can::Identifier identifier{Command::SYSTEM_COMMAND, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 5;
  data.data[4] = kSubcommandLocoEmergencyStop;
  callbacks_.tx->SendPacket(identifier, data);
}

void Station::SendSystemStop() {
  RR32Can::Identifier identifier{Command::SYSTEM_COMMAND, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 5;
  data.data[4] = kSubcommandSystemStop;
  callbacks_.tx->SendPacket(identifier, data);
}

void Station::SendSystemGo() {
  RR32Can::Identifier identifier{Command::SYSTEM_COMMAND, this->senderHash_};
  RR32Can::Data data;
  data.dlc = 5;
  data.data[4] = kSubcommandSystemGo;
  callbacks_.tx->SendPacket(identifier, data);
}

void Station::SendAccessoryPacket(RR32Can::MachineTurnoutAddress turnoutAddress, RailProtocol protocol,
                                  TurnoutDirection direction, uint8_t power) {
  RR32Can::Identifier identifier{Command::ACCESSORY_SWITCH, this->senderHash_};

  RR32Can::Data data;

  RR32Can::TurnoutPacket payload(data);
  payload.initData();
  turnoutAddress |= getAccessoryLocIdMask(protocol);
  payload.setLocid(turnoutAddress);  // Set the turnout address
  payload.setDirection(direction);   // Set the turnout direction
  payload.setPower(power);

#if (LOG_CAN_OUT_MSG == STD_ON)
  printf("Setting turnout %d to position %s %s\n", payload.locid & (~0x3000), payload.position == 0 ? "RED " : "GREEN",
         payload.power ? "(ON) " : "(OFF)");
#endif

  callbacks_.tx->SendPacket(identifier, data);
}

void Station::SendPacket(RR32Can::Identifier identifier, const RR32Can::Data & data) {
  identifier.setHash(this->senderHash_);
  callbacks_.tx->SendPacket(identifier, data);
}

void Station::HandlePacket(const RR32Can::Identifier& id, const RR32Can::Data& data) {
#if LOG_CAN_RAW_MSG_IN == STD_ON
  id.printAll();
  printf("\n");
#endif

  switch (id.getCommand()) {
    case RR32Can::Command::SYSTEM_COMMAND: {
      const SystemMessage msg(const_cast<RR32Can::Data&>(data));
      this->HandleSystemCommand(id, msg);
    } break;

    case RR32Can::Command::PING:
#if (LOG_PING == STD_ON)
      printf("Ping. Payload: 0x");
      data.printAsHex();
      printf("\n");
#endif
      break;

    case RR32Can::Command::ACCESSORY_SWITCH:
      printf("Accessory Switch. Details: ");
      this->HandleAccessoryPacket(data, id.isResponse());
      printf("\n");
      break;

    case RR32Can::Command::LOCO_DIRECTION:
      this->HandleLocoDirection(data);
      break;

    case RR32Can::Command::LOCO_SPEED:
      this->HandleLocoSpeed(data);
      break;

    case RR32Can::Command::LOCO_FUNCTION:
      this->HandleLocoFunction(data);
      break;

    case RR32Can::Command::REQUEST_CONFIG_DATA:
#if (LOG_CONFIG_DATA_STREAM_LEVEL >= LOG_CONFIG_DATA_STREAM_LEVEL_ALL)
      printf("Request Config Data. Payload: ");
      data.printAsText();
      printf("\n");
#endif
      break;

    case RR32Can::Command::CONFIG_DATA_STREAM:
#if (LOG_CONFIG_DATA_STREAM_LEVEL >= LOG_CONFIG_DATA_STREAM_LEVEL_ALL)
      printf("Config Data Stream.\n");
#endif
      this->HandleConfigDataStream(data);
      break;

    default:
      printf("Unknown or not implemented. Dump: 0x%#02x\n", id.getCommand());
      data.printAsHex();
      printf("\n");
      break;
  }
}

void Station::HandleAccessoryPacket(const RR32Can::Data& data, bool request) {
  // Create a const packet from const data by using some dangerous casts.
  const RR32Can::TurnoutPacket turnoutPacket(const_cast<RR32Can::Data&>(data));
  turnoutPacket.printAll();

  if (callbacks_.accessory != nullptr) {
    callbacks_.accessory->OnAccessoryPacket(turnoutPacket, request);
  }
}

Locomotive* Station::getLocoForUid(const RR32Can::Uid_t locid) {
  if (callbacks_.engine != nullptr) {
    return callbacks_.engine->getLoco(locid);
  } else {
    return nullptr;
  }
}

void Station::HandleLocoDirection(const RR32Can::Data& data) {
  if (data.dlc == 5) {
    // response.
    Locomotive* engine = getLocoForUid(data.getLocid());
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
    Locomotive::Uid_t uid = data.getLocid();
    Locomotive::Velocity_t velocity = (data.data[4] << 8) | data.data[5];
    if (callbacks_.engine != nullptr) {
      callbacks_.engine->setLocoVelocity(uid, velocity);
    }
  }
}

void Station::HandleLocoFunction(const RR32Can::Data& data) {
  if (data.dlc == 6) {
    Locomotive* engine = getLocoForUid(data.getLocid());
    if (engine == nullptr) {
      return;
    }
    engine->setFunction(data.data[4], data.data[5]);
  }
}

}  // namespace RR32Can
