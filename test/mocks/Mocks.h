#ifndef __MOCKS__MOCKS_H__
#define __MOCKS__MOCKS_H__

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "RR32Can/callback/AccessoryCbk.h"
#include "RR32Can/callback/ConfigDataCbk.h"
#include "RR32Can/callback/EngineCbk.h"
#include "RR32Can/callback/SystemCbk.h"
#include "RR32Can/callback/TxCbk.h"

namespace mocks {

class SystemCbkMock : public RR32Can::callback::SystemCbk {
 public:
  MOCK_METHOD(void, setSystemState, (bool), (override));
};

class EngineCbkMock : public RR32Can::callback::EngineCbk {
 public:
  MOCK_METHOD(RR32Can::Locomotive *, getLoco, (RR32Can::Locomotive::Uid_t), (override));
  MOCK_METHOD(void, setLocoVelocity, (RR32Can::Locomotive::Uid_t, RR32Can::Velocity_t), (override));
  MOCK_METHOD(void, setLocoVelocity, (RR32Can::Velocity_t), (override));
};

class ConfigDataCbkMock : public RR32Can::callback::ConfigDataCbk {
 public:
  MOCK_METHOD(void, startStream_Mock, (), ());
  MOCK_METHOD(void, addMessage, (const RR32Can::Data &), (override));
  MOCK_METHOD(void, reset, (), (override));

  void startStream() override {
    streamState = StreamState::WAITING_FIRST_PACKET;
    startStream_Mock();
  }
};

class AccessoryCbkMock : public RR32Can::callback::AccessoryCbk {
 public:
  MOCK_METHOD(void, OnAccessoryPacket, (RR32Can::TurnoutPacket & packet), (override));
};

class StationTxCbkMock : public RR32Can::callback::TxCbk {
 public:
  MOCK_METHOD(void, SendPacket, (const RR32Can::Identifier &, const RR32Can::Data &));
};

}  // namespace mocks

#endif  // __MOCKS__MOCKS_H__
