#include "core/Entity.hpp"

#include "core/Component.hpp"

#include <gtest/gtest.h>

namespace vq::tests::core::Entity {

/*----------------------------------------------------------------------------*/

class MockComponentBase : public vq::core::Component {
  public:
    MockComponentBase(vq::core::Entity& parent)
        : vq::core::Component(parent), m_render_called(false),
          m_update_called(false) {}

    inline bool render_called() const { return m_render_called; }
    inline bool update_called() const { return m_update_called; }

    void render() override { m_render_called = true; }
    void update(double delta_time) override { m_update_called = true; }

  private:
    bool m_render_called;
    bool m_update_called;
};

class MockComponent1 : public MockComponentBase {
  public:
    using MockComponentBase::MockComponentBase;
};

class MockComponent2 : public MockComponentBase {
  public:
    using MockComponentBase::MockComponentBase;
};

/*----------------------------------------------------------------------------*/

TEST(Core_Entity, entity_id_is_correct) {
    vq::core::Entity entity("test_entity");
    ASSERT_STREQ(entity.get_id().c_str(), "test_entity");
}

/*----------------------------------------------------------------------------*/

TEST(Core_Entity, entity_add_component) {
    vq::core::Entity entity("test_entity");
    ASSERT_TRUE(entity.add_component<MockComponent1>());
    ASSERT_TRUE(entity.add_component<MockComponent2>());
    ASSERT_FALSE(entity.add_component<MockComponent1>());
}

/*----------------------------------------------------------------------------*/

TEST(Core_Entity, entity_get_component) {
    vq::core::Entity entity("test_entity");
    entity.add_component<MockComponent1>();
    auto component_1 = entity.get_component<MockComponent1>();
    ASSERT_FALSE(component_1.expired());
    auto component_2 = entity.get_component<MockComponent2>();
    ASSERT_TRUE(component_2.expired());
}

/*----------------------------------------------------------------------------*/

TEST(Core_Entity, entity_remove_component) {
    vq::core::Entity entity("test_entity");
    entity.add_component<MockComponent1>();
    auto component_1 = entity.get_component<MockComponent1>();
    ASSERT_FALSE(component_1.expired());
    ASSERT_TRUE(entity.remove_component<MockComponent1>());
    ASSERT_TRUE(component_1.expired());
    ASSERT_FALSE(entity.remove_component<MockComponent2>());
}

/*----------------------------------------------------------------------------*/

TEST(Core_Entity, entity_render_called_for_all_components) {
    vq::core::Entity entity("test_entity");
    entity.add_component<MockComponent1>();
    entity.add_component<MockComponent2>();
    ASSERT_FALSE(
        entity.get_component<MockComponent1>().lock()->render_called());
    ASSERT_FALSE(
        entity.get_component<MockComponent2>().lock()->render_called());
    entity.render();
    ASSERT_TRUE(entity.get_component<MockComponent1>().lock()->render_called());
    ASSERT_TRUE(entity.get_component<MockComponent2>().lock()->render_called());
}

/*----------------------------------------------------------------------------*/

TEST(Core_Entity, entity_update_called_for_all_components) {
    vq::core::Entity entity("test_entity");
    entity.add_component<MockComponent1>();
    entity.add_component<MockComponent2>();
    ASSERT_FALSE(
        entity.get_component<MockComponent1>().lock()->update_called());
    ASSERT_FALSE(
        entity.get_component<MockComponent2>().lock()->update_called());
    entity.update(0.0);
    ASSERT_TRUE(entity.get_component<MockComponent1>().lock()->update_called());
    ASSERT_TRUE(entity.get_component<MockComponent2>().lock()->update_called());
}

/*----------------------------------------------------------------------------*/

} // namespace vq::tests::core::Entity
