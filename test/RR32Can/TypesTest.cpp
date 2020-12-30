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
  typename AddressTypeFixture<TypeParam>::HumanAddress m = 1;
  EXPECT_EQ(m.value(), 1);

  typename AddressTypeFixture<TypeParam>::MachineAddress n = typename AddressTypeFixture<TypeParam>::MachineAddress(m);
  EXPECT_EQ(n.value(), 0);

  typename AddressTypeFixture<TypeParam>::HumanAddress o = n;
  EXPECT_EQ(o.value(), 1);

  typename AddressTypeFixture<TypeParam>::HumanAddress p = m;
  EXPECT_EQ(p.value(), 1);
}
REGISTER_TYPED_TEST_SUITE_P(AddressTypeFixture, HumanMachineAddressConversion);

using TurnoutTypes = std::tuple<HumanTurnoutAddress, MachineTurnoutAddress>;
INSTANTIATE_TYPED_TEST_SUITE_P(Turnout, AddressTypeFixture, TurnoutTypes);

using LocomotiveTypes = std::tuple<HumanLocomotiveAddress, MachineLocomotiveAddress>;
INSTANTIATE_TYPED_TEST_SUITE_P(Locomotive, AddressTypeFixture, LocomotiveTypes);

}  // namespace RR32Can
