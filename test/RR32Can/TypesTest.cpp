#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <tuple>

#include "RR32Can/Types.h"

namespace RR32Can {

template <typename T>
class AddressTypeFixture : public ::testing::Test {
 public:
  using HumanAddress = typename std::tuple_element<0, T>::type;
  using MachineAddress = typename std::tuple_element<1, T>::type;
};

TYPED_TEST_SUITE_P(AddressTypeFixture);

TYPED_TEST_P(AddressTypeFixture, HumanMachineAddressConversion) {
  typename AddressTypeFixture<TypeParam>::HumanAddress m{1};
  EXPECT_EQ(m.value(), 1);

  typename AddressTypeFixture<TypeParam>::MachineAddress n = typename AddressTypeFixture<TypeParam>::MachineAddress(m);
  EXPECT_EQ(n.value(), 0);

  typename AddressTypeFixture<TypeParam>::HumanAddress o = n;
  EXPECT_EQ(o.value(), 1);

  typename AddressTypeFixture<TypeParam>::HumanAddress p = m;
  EXPECT_EQ(p.value(), 1);
}

TYPED_TEST_P(AddressTypeFixture, AddressWithProtocol) {
  typename AddressTypeFixture<TypeParam>::MachineAddress protoAddr{0xC042};  // DCC Engine 66
  EXPECT_EQ(protoAddr.value(), 0xC042);
  EXPECT_EQ(protoAddr.getNumericAddress().value(), 66);
}

TEST(TurnoutAddress, AddProtocol) {
  MachineTurnoutAddress addr{66};
  EXPECT_EQ(addr.value(), 66);

  {
    MachineTurnoutAddress mm2addr{66};
    mm2addr.setProtocol(RailProtocol::MM2);
    ASSERT_EQ(mm2addr.value(), 0x3042);
  }

  addr.setProtocol(RailProtocol::DCC);
  EXPECT_EQ(addr.value(), 0x3842);
}

TEST(LocomotiveAddress, AddProtocol) {
  MachineLocomotiveAddress addr{66};
  EXPECT_EQ(addr.value(), 66);
  addr.setProtocol(RailProtocol::MM2);
  ASSERT_EQ(addr.value(), 66);

  addr.setProtocol(RailProtocol::DCC);
  EXPECT_EQ(addr.value(), 0xC042);
}

REGISTER_TYPED_TEST_SUITE_P(AddressTypeFixture, HumanMachineAddressConversion, AddressWithProtocol);

using TurnoutTypes = std::tuple<HumanTurnoutAddress, MachineTurnoutAddress>;
INSTANTIATE_TYPED_TEST_SUITE_P(Turnout, AddressTypeFixture, TurnoutTypes);

using LocomotiveTypes = std::tuple<HumanLocomotiveAddress, MachineLocomotiveAddress>;
INSTANTIATE_TYPED_TEST_SUITE_P(Locomotive, AddressTypeFixture, LocomotiveTypes);

}  // namespace RR32Can
