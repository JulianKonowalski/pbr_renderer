#include "core/Resource.hpp"

#include <gtest/gtest.h>

namespace vq::tests::core::Resource {

/*----------------------------------------------------------------------------*/

class MockResource : public vq::core::Resource {
  public:
    MockResource(const std::string& id) : vq::core::Resource(id) {}
    ~MockResource() override = default;

  protected:
    bool do_load() override { return true; }
    void do_unload() override {}
};

/*----------------------------------------------------------------------------*/

TEST(Core_Resource, resource_id_is_correct) {
    MockResource mock_resource("mock_resource");
    ASSERT_STREQ(mock_resource.get_id().c_str(), "mock_resource");
}

/*----------------------------------------------------------------------------*/

TEST(Core_Resource, load_and_unload_are_called) {
    MockResource mock_resource("mock_resource");
    ASSERT_FALSE(mock_resource.is_loaded());

    mock_resource.load();
    ASSERT_TRUE(mock_resource.is_loaded());

    mock_resource.unload();
    ASSERT_FALSE(mock_resource.is_loaded());
}

/*----------------------------------------------------------------------------*/

} // namespace vq::tests::core::Resource
