#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

#include "RR32Can/StlAdapter.h"

#include "RR32Can/Station.h"

#include "RR32Can/Constants.h"
#include "RR32Can/messages/CanFrame.h"
#include "RR32Can/messages/TurnoutPacket.h"
#include "RR32Can/util/constexpr.h"
#include "RR32Can/util/utils.h"

#include "RR32Can_config.h"

namespace RR32Can {

void Station::begin(const uint16_t stationUUID, CallbackStruct& callbacks) {
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
          callbacks_.engine->setLocoVelocity(0);
        }
      } break;
      case RR32Can::SystemSubcommand::SYSTEM_STOP:
        printf("STOP!\n");
        if (callbacks_.system != nullptr) {
          callbacks_.system->setSystemState(false, id.isResponse());
        }
        break;
      case SystemSubcommand::LOCO_EMERGENCY_STOP: {
        Locomotive::Uid_t uid = msg.getLocid();
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

void Station::RequestEngineList(const uint8_t offset, callback::ConfigDataCbk* configDataConsumer) {
  AbortCurrentConfigRequest();
  callbacks_.configData = configDataConsumer;

  /* First packet */
  CanFrame frame1{Identifier{Command::REQUEST_CONFIG_DATA, senderHash_}, {}};
  frame1.data.dlc = 8;
  strncpy(frame1.data.dataAsString(), RR32Can::Filenames::kEngineNames, Data::kDataBufferLength);

  /* Second packet */
  CanFrame frame2{Identifier{Command::REQUEST_CONFIG_DATA, senderHash_}, {}};
  frame2.data.dlc =
      snprintf(frame2.data.dataAsString(), Data::kDataBufferLength, "%d %d", offset, kNumEngineNamesDownload);
  if (frame2.data.dlc <= CanDataMaxLength) {
    // expectedConfigData = ConfigDataStreamType::LOKNAMEN;
    if (callbacks_.configData != nullptr) {
      callbacks_.configData->startStream();
    }
    callbacks_.tx->SendPacket(frame1);
    callbacks_.tx->SendPacket(frame2);
  } else {
    // Packet was oversized. Abort the operation.
    return;
  }
}

void Station::RequestEngine(const LocomotiveShortInfo& engine, callback::ConfigDataCbk* configDataConsumer) {
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

  CanFrame frame{{Command::REQUEST_CONFIG_DATA, senderHash_}, {}};

  /* First packet */
  frame.data.dlc = 8;
  strncpy(frame.data.dataAsString(), RR32Can::Filenames::kEngine, Data::kDataBufferLength);
  callbacks_.tx->SendPacket(frame);

  /* Second packet */
  frame.data.reset();
  frame.data.dlc = 8;
  const char* engineName = engine.getName();
  const auto engineNameLength = strlen(engineName);
  strncpy(frame.data.dataAsString(), engineName, CanDataMaxLength);

  callbacks_.tx->SendPacket(frame);

  /* Third packet */
  frame.data.reset();
  frame.data.dlc = 8;
  if (engineNameLength > CanDataMaxLength) {
    strncpy(frame.data.dataAsString(), engineName + CanDataMaxLength, CanDataMaxLength);
  }

  callbacks_.tx->SendPacket(frame);
}

void Station::RequestEngineDirection(const LocomotiveData& engine) {
  CanFrame frame{{Command::LOCO_DIRECTION, this->senderHash_}, {}};
  frame.data.dlc = 4;
  frame.data.setLocid(engine.getUid());

  callbacks_.tx->SendPacket(frame);
}

void Station::SendEngineDirection(const LocomotiveData& engine, const EngineDirection direction) {
  if ((direction == EngineDirection::FORWARD) || (direction == EngineDirection::REVERSE) ||
      (direction == EngineDirection::CHANGE_DIRECTION)) {
    CanFrame frame{util::LocoDirection(false, engine.getUid(), direction)};
    SendPacket(frame);
  }  // else: not implemented.
}

void Station::RequestEngineVelocity(const LocomotiveData& engine) {
  CanFrame frame{{Command::LOCO_SPEED, this->senderHash_}, {}};
  frame.data.dlc = 4;
  frame.data.setLocid(engine.getUid());

  callbacks_.tx->SendPacket(frame);
}

void Station::SendEngineVelocity(const LocomotiveData& engine, const LocomotiveData::Velocity_t velocity) {
  CanFrame frame{util::LocoSpeed(false, engine.getUid(), velocity)};

  SendPacket(frame);
}

void Station::RequestEngineFunction(const LocomotiveData& engine, const uint8_t function) {
  CanFrame frame{{Command::LOCO_FUNCTION, this->senderHash_}, {}};
  frame.data.dlc = 5;
  frame.data.setLocid(engine.getUid());
  frame.data.data[4] = function;

  callbacks_.tx->SendPacket(frame);
}

void Station::RequestEngineAllFunctions(const LocomotiveData& engine) {
  CanFrame frame{{Command::LOCO_FUNCTION, this->senderHash_}, {}};
  frame.data.dlc = 5;
  frame.data.setLocid(engine.getUid());
  for (uint8_t i = 0; i < 16; ++i) {
    frame.data.data[4] = i;
    callbacks_.tx->SendPacket(frame);
  }
}

void Station::SendEngineFunction(const LocomotiveData& engine, const uint8_t function, const bool value) {
  CanFrame frame{util::LocoFunction(false, engine.getUid(), function, value)};
  SendPacket(frame);
}

void Station::SendEmergencyStop(const LocomotiveData& engine) {
  CanFrame frame{{Command::SYSTEM_COMMAND, this->senderHash_}, {}};
  SystemMessage payload{frame.data};
  payload.initData();
  payload.setSubcommand(SystemSubcommand::LOCO_EMERGENCY_STOP);
  payload.setLocid(engine.getUid());
  callbacks_.tx->SendPacket(frame);
}

void Station::SendSystemStop() {
  CanFrame frame{{Command::SYSTEM_COMMAND, this->senderHash_}, {}};
  frame.data.dlc = 5;
  frame.data.data[4] = kSubcommandSystemStop;
  callbacks_.tx->SendPacket(frame);
}

void Station::SendSystemGo() {
  CanFrame frame{{Command::SYSTEM_COMMAND, this->senderHash_}, {}};
  frame.data.dlc = 5;
  frame.data.data[4] = kSubcommandSystemGo;
  callbacks_.tx->SendPacket(frame);
}

void Station::SendAccessoryPacket(RR32Can::MachineTurnoutAddress turnoutAddress, const RailProtocol protocol,
                                  const TurnoutDirection direction, const uint8_t power) {
  CanFrame frame{{Command::ACCESSORY_SWITCH, this->senderHash_}, {}};

  RR32Can::TurnoutPacket payload(frame.data);
  payload.initData();
  turnoutAddress |= getAccessoryLocIdMask(protocol);
  payload.setLocid(turnoutAddress);  // Set the turnout address
  payload.setDirection(direction);   // Set the turnout direction
  payload.setPower(power);

#if (LOG_CAN_OUT_MSG == STD_ON)
  printf("Setting turnout %u to position %s %s\n", payload.getLocid().value() & (~0x3000),
         payload.getDirection() == TurnoutDirection::RED ? "RED " : "GREEN", payload.getPower() ? "(ON) " : "(OFF)");
#endif

  callbacks_.tx->SendPacket(frame);
}

void Station::SendPacket(RR32Can::CanFrame& canFrame) {
  canFrame.id.setHash(this->senderHash_);
  callbacks_.tx->SendPacket(canFrame);
}

void Station::HandlePacket(const RR32Can::CanFrame& canFrame) {
#if LOG_CAN_RAW_MSG_IN == STD_ON
  canFrame.id.printAll();
  printf("\n");
#endif

  switch (canFrame.id.getCommand()) {
    case RR32Can::Command::SYSTEM_COMMAND: {
      const SystemMessage msg(const_cast<RR32Can::Data&>(canFrame.data));
      this->HandleSystemCommand(canFrame.id, msg);
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
      this->HandleAccessoryPacket(canFrame.data, canFrame.id.isResponse());
      printf("\n");
      break;

    case RR32Can::Command::LOCO_DIRECTION:
      this->HandleLocoDirection(canFrame.data);
      break;

    case RR32Can::Command::LOCO_SPEED:
      this->HandleLocoSpeed(canFrame.data);
      break;

    case RR32Can::Command::LOCO_FUNCTION:
      this->HandleLocoFunction(canFrame.data);
      break;

    case RR32Can::Command::REQUEST_CONFIG_DATA:
#if (LOG_CONFIG_DATA_STREAM_LEVEL >= LOG_CONFIG_DATA_STREAM_LEVEL_ALL)
      printf("Request Config Data. Payload: ");
      canFrame.data.printAsText();
      printf("\n");
#endif
      break;

    case RR32Can::Command::CONFIG_DATA_STREAM:
#if (LOG_CONFIG_DATA_STREAM_LEVEL >= LOG_CONFIG_DATA_STREAM_LEVEL_ALL)
      printf("Config Data Stream.\n");
#endif
      this->HandleConfigDataStream(canFrame.data);
      break;

    default:
      printf("Unknown or not implemented. Dump: %#02x\n", static_cast<CommandByte_t>(canFrame.id.getCommand()));
      canFrame.data.printAsHex();
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

void Station::HandleLocoDirection(const RR32Can::Data& data) {
  const bool isResponse = data.dlc == 5;
  if (isResponse) {
    // response.
    const Locomotive::Uid_t uid = data.getLocid();
    const EngineDirection direction = static_cast<EngineDirection>(data.data[4]);
    if (direction == EngineDirection::FORWARD || direction == EngineDirection::REVERSE) {
      callbacks_.engine->setLocoDirection(uid, direction);
    } else if (direction == EngineDirection::CHANGE_DIRECTION) {
      callbacks_.engine->changeLocoDirection(uid);
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
    const Locomotive::Uid_t uid = data.getLocid();
    callbacks_.engine->setLocoFunction(uid, data.data[4], data.data[5]);
  }
}

}  // namespace RR32Can
