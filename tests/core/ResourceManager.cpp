#include "core/ResourceManager.hpp"

#include <gtest/gtest.h>

namespace vq::tests::core::ResourceManager {

/*----------------------------------------------------------------------------*/

class MockResourceEmpty : public vq::core::Resource {
  public:
    explicit MockResourceEmpty(const std::string& id)
        : vq::core::Resource(id, "") {}
    ~MockResourceEmpty() override = default;

    MockResourceEmpty(MockResourceEmpty&& other) : vq::core::Resource("", "") {
        *this = std::move(other);
    }

    MockResourceEmpty& operator=(MockResourceEmpty&& other) {
        vq::core::Resource::operator=(std::move(other));
        return *this;
    }

  protected:
    bool do_load() noexcept override { return true; }
    bool do_reload() noexcept override { return true; }
    void do_unload() noexcept override {}
};

class MockResourceWithTestValue : public vq::core::Resource {
  public:
    explicit MockResourceWithTestValue(const std::string& id,
                                       const int test_value)
        : vq::core::Resource(id, ""), m_test_value(test_value) {}
    ~MockResourceWithTestValue() override = default;

    MockResourceWithTestValue(MockResourceWithTestValue&& other)
        : vq::core::Resource("", "") {
        *this = std::move(other);
    }

    MockResourceWithTestValue& operator=(MockResourceWithTestValue&& other) {
        vq::core::Resource::operator=(std::move(other));
        return *this;
    }

    inline int get_test_value() const { return m_test_value; }
    inline void set_test_value(const int value) { m_test_value = value; }

  protected:
    bool do_load() noexcept override { return true; }
    bool do_reload() noexcept override { return true; }
    void do_unload() noexcept override {}

  private:
    int m_test_value;
};

/*----------------------------------------------------------------------------*/

TEST(Core_ResourceManager, loads_resource) {
    auto resource_manager = vq::core::ResourceManager::get_instance();
    ASSERT_TRUE(resource_manager.load_resource<MockResourceEmpty>(
        "test_loads_resource_1"));
    ASSERT_FALSE(resource_manager.load_resource<MockResourceEmpty>(
        "test_loads_resource_1"));
    ASSERT_TRUE(resource_manager.load_resource<MockResourceEmpty>(
        "test_loads_resource_2"));
    ASSERT_TRUE( // duplicated ID but a different data type
        resource_manager.load_resource<MockResourceWithTestValue>(
            "test_loads_resource_1", 1));
}

/*----------------------------------------------------------------------------*/

TEST(Core_ResourceManager, gets_resource) {
    auto resource_manager = vq::core::ResourceManager::get_instance();

    auto resource_handle =
        resource_manager.get_resource<MockResourceWithTestValue>(
            "test_gets_resource");
    ASSERT_FALSE(resource_handle.is_valid());

    resource_manager.load_resource<MockResourceWithTestValue>(
        "test_gets_resource", 1);
    resource_handle = resource_manager.get_resource<MockResourceWithTestValue>(
        "test_gets_resource");
    ASSERT_TRUE(resource_handle.is_valid());
    ASSERT_EQ(resource_handle.get().get_test_value(), 1);
}

/*----------------------------------------------------------------------------*/

TEST(Core_ResourceManager, modifies_resource) {
    auto resource_manager = vq::core::ResourceManager::get_instance();

    resource_manager.load_resource<MockResourceWithTestValue>(
        "test_modifies_resource", 1);
    auto resource_handle_1 =
        resource_manager.get_resource<MockResourceWithTestValue>(
            "test_modifies_resource");
    ASSERT_EQ(resource_handle_1.get().get_test_value(), 1);

    resource_handle_1.get().set_test_value(2);
    ASSERT_EQ(resource_handle_1.get().get_test_value(), 2);

    auto resource_handle_2 =
        resource_manager.get_resource<MockResourceWithTestValue>(
            "test_modifies_resource");
    ASSERT_EQ(resource_handle_2.get().get_test_value(), 2);

    resource_handle_2.get().set_test_value(3);
    ASSERT_EQ(resource_handle_1.get().get_test_value(), 3);
    ASSERT_EQ(resource_handle_2.get().get_test_value(), 3);
}

/*----------------------------------------------------------------------------*/

TEST(Core_ResourceManager, gets_resource_user_count) {
    auto resource_manager = vq::core::ResourceManager::get_instance();

    resource_manager.load_resource<MockResourceEmpty>(
        "test_gets_resource_user_count");
    auto resource_handle_1 = resource_manager.get_resource<MockResourceEmpty>(
        "test_gets_resource_user_count");

    ASSERT_EQ(resource_manager.get_resource_user_count<MockResourceEmpty>(
                  "test_gets_resource_user_count"),
              1);

    {
        auto resource_handle_2 =
            resource_manager.get_resource<MockResourceEmpty>(
                "test_gets_resource_user_count");
        ASSERT_EQ(resource_manager.get_resource_user_count<MockResourceEmpty>(
                      "test_gets_resource_user_count"),
                  2);
    }

    ASSERT_EQ(resource_manager.get_resource_user_count<MockResourceEmpty>(
                  "test_gets_resource_user_count"),
              1);
}

/*----------------------------------------------------------------------------*/

/**
 * Resource gets deleted only after it had AT LEAST ONE user. If it's loaded but
 * never used, it will hang around pretty much forever. I will probably change
 * it later or add a dedicated method to ResourceManager to purge all of the
 * unused resources
 */
TEST(Core_ResourceManager, deletes_unreferenced_resource) {
    auto resource_manager = vq::core::ResourceManager::get_instance();

    resource_manager.load_resource<MockResourceEmpty>(
        "test_deletes_unreferenced_resource");

    {
        auto resource_handle = resource_manager.get_resource<MockResourceEmpty>(
            "test_deletes_unreferenced_resource");
        ASSERT_TRUE(resource_handle.is_valid());
    }

    auto resource_handle = resource_manager.get_resource<MockResourceEmpty>(
        "test_deletes_unreferenced_resource");
    ASSERT_FALSE(resource_handle.is_valid());
}

/*----------------------------------------------------------------------------*/

} // namespace vq::tests::core::ResourceManager
