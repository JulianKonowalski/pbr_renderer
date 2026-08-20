#include "core/Resource.hpp"

#include <gtest/gtest.h>

namespace vq::tests::core::Resource {

/*----------------------------------------------------------------------------*/

class MockResource : public vq::core::Resource {
  public:
    explicit MockResource(const std::string& id)
        : vq::core::Resource(id), m_is_reloaded(false) {}
    ~MockResource() override = default;

    MockResource(MockResource&& other) : vq::core::Resource("") {
        *this = std::move(other);
    }

    MockResource& operator=(MockResource&& other) {
        vq::core::Resource::operator=(std::move(other));
        return *this;
    }

    inline bool is_reloaded() const { return m_is_reloaded; }

  protected:
    bool do_load() noexcept override { return true; }
    bool do_reload() noexcept override {
        m_is_reloaded = true;
        return true;
    }
    void do_unload() noexcept override {}

  private:
    bool m_is_reloaded;
};

class MockResourceEmpty : public vq::core::Resource {
  public:
    explicit MockResourceEmpty(const std::string& id)
        : vq::core::Resource(id) {}
    ~MockResourceEmpty() override = default;

    MockResourceEmpty(MockResourceEmpty&& other) : vq::core::Resource("") {
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
        : vq::core::Resource(id), m_test_value(test_value) {}
    ~MockResourceWithTestValue() override = default;

    MockResourceWithTestValue(MockResourceWithTestValue&& other)
        : vq::core::Resource("") {
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

TEST(Core_Resource, resource_move_constructor) {
    MockResource mock_resource_1("mock_resource_1");
    ASSERT_STREQ(mock_resource_1.get_id().c_str(), "mock_resource_1");

    MockResource mock_resource_2(std::move(mock_resource_1));
    ASSERT_STREQ(mock_resource_1.get_id().c_str(), "");
    ASSERT_STREQ(mock_resource_2.get_id().c_str(), "mock_resource_1");
}

/*----------------------------------------------------------------------------*/

TEST(Core_Resource, resource_move_assignment_operator) {
    MockResource mock_resource_1("mock_resource_1");
    ASSERT_STREQ(mock_resource_1.get_id().c_str(), "mock_resource_1");

    MockResource mock_resource_2 = std::move(mock_resource_1);
    ASSERT_STREQ(mock_resource_1.get_id().c_str(), "");
    ASSERT_STREQ(mock_resource_2.get_id().c_str(), "mock_resource_1");
}

/*----------------------------------------------------------------------------*/

TEST(Core_Resource, resource_id_is_correct) {
    MockResource mock_resource("mock_resource");
    ASSERT_STREQ(mock_resource.get_id().c_str(), "mock_resource");
}

/*----------------------------------------------------------------------------*/

TEST(Core_Resource, resource_load_reload_and_unload_are_called) {
    MockResource mock_resource("mock_resource");
    ASSERT_FALSE(mock_resource.is_loaded());
    ASSERT_FALSE(mock_resource.is_reloaded());

    ASSERT_FALSE(mock_resource.reload());
    ASSERT_FALSE(mock_resource.is_loaded());

    ASSERT_TRUE(mock_resource.load());
    ASSERT_TRUE(mock_resource.is_loaded());

    ASSERT_TRUE(mock_resource.reload());
    ASSERT_TRUE(mock_resource.is_reloaded());

    mock_resource.unload();
    ASSERT_FALSE(mock_resource.is_loaded());
}

/*----------------------------------------------------------------------------*/

TEST(Core_Resource, resource_manager_loads_resource) {
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

TEST(Core_Resource, resource_manager_gets_resource) {
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

TEST(Core_Resource, resource_manager_modifies_resource) {
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

/**
 * Resource gets deleted only after it had AT LEAST ONE user. If it's loaded but
 * never used, it will hang around pretty much forever. I will probably change
 * it later or add a dedicated method to ResourceManager to purge all of the
 * unused resources
 */
TEST(Core_Resource, resource_manager_deletes_unreferenced_resource) {
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

} // namespace vq::tests::core::Resource
