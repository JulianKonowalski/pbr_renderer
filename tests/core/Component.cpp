#include "core/Component.hpp"

#include "core/Entity.hpp"

#include <gtest/gtest.h>

namespace vq::tests::core::component {

/*----------------------------------------------------------------------------*/

class MockComponent1 : public vq::core::Component {
  public:
    using Component::Component;
    void render(void) override {}
    void update(double delta_time) override {}
};

class MockComponent2 : public vq::core::Component {
  public:
    using Component::Component;
    void render(void) override {}
    void update(double delta_time) override {}
};

/*----------------------------------------------------------------------------*/

TEST(Core_Component, component_id_is_unique) {
    ASSERT_NE(vq::core::Component::get_type_id<MockComponent1>(),
              vq::core::Component::get_type_id<MockComponent2>());
}

/*----------------------------------------------------------------------------*/

TEST(Core_Component, component_parent_is_correct) {
    vq::core::Entity entity("test_entity");
    MockComponent1 component(entity);
    ASSERT_EQ(&(component.get_parent()), &entity);
}

/*----------------------------------------------------------------------------*/

} // namespace vq::tests::core::component
