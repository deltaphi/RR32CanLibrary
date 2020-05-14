#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "RR32Can/Station.h"
#include "RR32Can/StationCbk.h"

#include "RR32Can/Locomotive.h"

namespace RR32Can {
void SendPacket(Identifier const &,
                Data const &) {  // TODO: Replace with mockable interface.
}
}  // namespace RR32Can

namespace integration {

class StationCbkMock : public RR32Can::StationCbk {
 public:
  MOCK_METHOD(RR32Can::Locomotive *, getLoco, (RR32Can::Locomotive::Uid_t),
              (override));
  MOCK_METHOD(void, setLocoVelocity,
              (RR32Can::Locomotive::Uid_t, RR32Can::Velocity_t), (override));
  MOCK_METHOD(void, setLocoVelocity, (RR32Can::Velocity_t), (override));
  MOCK_METHOD(void, setSystemState, (bool), (override));
};

class StationTestFixture : public ::testing::Test {
  void SetUp() { station.begin(0, callback); }

  void TearDown() {}

 public:
  ::testing::StrictMock<StationCbkMock> callback;
  RR32Can::Station station;
};

TEST_F(StationTestFixture, RecvStop) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command = RR32Can::kSystemCommand;
  data.dlc = 5;
  data.data[4] = RR32Can::kSubcommandSystemStop;

  EXPECT_CALL(callback, setSystemState(false));

  station.HandlePacket(id, data);

}

TEST_F(StationTestFixture, RecvGo) {
  RR32Can::Identifier id;
  RR32Can::Data data;

  id.command = RR32Can::kSystemCommand;
  data.dlc = 5;
  data.data[4] = RR32Can::kSubcommandSystemGo;

  EXPECT_CALL(callback, setSystemState(true));

  station.HandlePacket(id, data);
}

}  // namespace integration