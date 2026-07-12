#include "core/TypeId.hpp"

#include <gtest/gtest.h>

/*----------------------------------------------------------------------------*/

class MockIdableBase1 {};

class MockIdableChild11 final : public MockIdableBase1 {};

class MockIdableChild12 final : public MockIdableBase1 {};

/*----------------------------------------------------------------------------*/

class MockIdableBase2 {};

class MockIdableChild21 final : public MockIdableBase2 {};

class MockIdableChild22 final : public MockIdableBase2 {};

/*----------------------------------------------------------------------------*/

TEST(Core_TypeId, type_id_starts_at_0) {
    const auto id =
        vq::core::TypeId<MockIdableBase1>::get_type_id<MockIdableChild11>();
    ASSERT_EQ(id, 0);
}

/*----------------------------------------------------------------------------*/

TEST(Core_TypeId, type_id_unique_for_same_base_class) {
    const auto id_1 =
        vq::core::TypeId<MockIdableBase1>::get_type_id<MockIdableChild11>();
    const auto id_2 =
        vq::core::TypeId<MockIdableBase1>::get_type_id<MockIdableChild12>();
    ASSERT_NE(id_1, id_2);
}

/*----------------------------------------------------------------------------*/

TEST(Core_TypeId, type_id_increments_by_one) {
    const auto id_1 =
        vq::core::TypeId<MockIdableBase1>::get_type_id<MockIdableChild11>();
    const auto id_2 =
        vq::core::TypeId<MockIdableBase1>::get_type_id<MockIdableChild12>();
    ASSERT_EQ(id_2 - id_1, 1);
}

/*----------------------------------------------------------------------------*/

TEST(Core_TypeId, type_id_independent_for_different_base_classes) {
    const auto id_11 =
        vq::core::TypeId<MockIdableBase1>::get_type_id<MockIdableChild11>();
    const auto id_12 =
        vq::core::TypeId<MockIdableBase1>::get_type_id<MockIdableChild12>();
    const auto id_21 =
        vq::core::TypeId<MockIdableBase2>::get_type_id<MockIdableChild21>();
    const auto id_22 =
        vq::core::TypeId<MockIdableBase2>::get_type_id<MockIdableChild22>();
    ASSERT_EQ(id_11, 0);
    ASSERT_EQ(id_21, 0);
    ASSERT_EQ(id_12 - id_11, 1);
    ASSERT_EQ(id_22 - id_21, 1);
}

/*----------------------------------------------------------------------------*/
