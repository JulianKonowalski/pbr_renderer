#include "core/Resource.hpp"

#include <gtest/gtest.h>

namespace vq::tests::core::Resource {

/*----------------------------------------------------------------------------*/

class MockResource : public vq::core::Resource {
  public:
    MockResource(const std::string& id) : vq::core::Resource(id) {}
    ~MockResource() override = default;

    void load() override {}
    void unload() override {}
};

/*----------------------------------------------------------------------------*/

TEST(Core_Resource, resource_id_is_correct) {
    MockResource mock_resource("mock_resource");
    ASSERT_STREQ(mock_resource.get_id().c_str(), "mock_resource");
}

/*----------------------------------------------------------------------------*/

} // namespace vq::tests::core::Resource
