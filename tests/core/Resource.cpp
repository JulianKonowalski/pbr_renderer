#include "core/Resource.hpp"

#include <gtest/gtest.h>

namespace vq::tests::core::Resource {

/*----------------------------------------------------------------------------*/

class MockResource : public vq::core::Resource {
  public:
    using vq::core::Resource::Resource;

    MockResource(MockResource&& other) : vq::core::Resource("") {
        *this = std::move(other);
    }

    MockResource& operator=(MockResource&& other) {
        vq::core::Resource::operator=(std::move(other));
        return *this;
    }

    ~MockResource() override = default;

  protected:
    bool do_load() noexcept override { return true; }
    void do_unload() noexcept override {}
};

/*----------------------------------------------------------------------------*/

TEST(Core_Resource, resource_move_constructor) {
    MockResource mock_resource_1("mock_resource_1", "/test_path.obj");
    ASSERT_STREQ(mock_resource_1.get_id().c_str(), "mock_resource_1");
    ASSERT_STREQ(mock_resource_1.get_asset_path().c_str(), "/test_path.obj");

    MockResource mock_resource_2(std::move(mock_resource_1));
    ASSERT_STREQ(mock_resource_1.get_id().c_str(), "");
    ASSERT_STREQ(mock_resource_1.get_asset_path().c_str(), "");
    ASSERT_STREQ(mock_resource_2.get_id().c_str(), "mock_resource_1");
    ASSERT_STREQ(mock_resource_2.get_asset_path().c_str(), "/test_path.obj");
}

/*----------------------------------------------------------------------------*/

TEST(Core_Resource, resource_move_assignment_operator) {
    MockResource mock_resource_1("mock_resource_1", "/test_path.obj");
    ASSERT_STREQ(mock_resource_1.get_id().c_str(), "mock_resource_1");
    ASSERT_STREQ(mock_resource_1.get_asset_path().c_str(), "/test_path.obj");

    MockResource mock_resource_2 = std::move(mock_resource_1);
    ASSERT_STREQ(mock_resource_1.get_id().c_str(), "");
    ASSERT_STREQ(mock_resource_1.get_asset_path().c_str(), "");
    ASSERT_STREQ(mock_resource_2.get_id().c_str(), "mock_resource_1");
    ASSERT_STREQ(mock_resource_2.get_asset_path().c_str(), "/test_path.obj");
}

/*----------------------------------------------------------------------------*/

TEST(Core_Resource, resource_id_is_correct) {
    MockResource mock_resource("mock_resource");
    ASSERT_STREQ(mock_resource.get_id().c_str(), "mock_resource");
}

/*----------------------------------------------------------------------------*/

TEST(Core_Resource, resource_asset_path_is_correct) {
    MockResource mock_resource_1("mock_resource_1");
    ASSERT_STREQ(mock_resource_1.get_asset_path().c_str(), "");

    MockResource mock_resource_2("mock_resource_2", "/test_path.obj");
    ASSERT_STREQ(mock_resource_2.get_asset_path().c_str(), "/test_path.obj");
}

/*----------------------------------------------------------------------------*/

TEST(Core_Resource, resource_load_and_unload_are_called) {
    MockResource mock_resource("mock_resource");
    ASSERT_FALSE(mock_resource.is_loaded());

    mock_resource.load();
    ASSERT_TRUE(mock_resource.is_loaded());

    mock_resource.unload();
    ASSERT_FALSE(mock_resource.is_loaded());
}

/*----------------------------------------------------------------------------*/

} // namespace vq::tests::core::Resource
