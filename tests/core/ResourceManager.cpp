#include "core/ResourceManager.hpp"

#include <gtest/gtest.h>

namespace vq::tests::core::ResourceManager {

/*----------------------------------------------------------------------------*/

class MockResource : public vq::core::Resource {
  protected:
    bool do_load() noexcept override { return true; }
    bool do_reload() noexcept override { return true; }
    void do_unload() noexcept override {}
};

/*----------------------------------------------------------------------------*/

TEST(Core_ResourceManager, ) {}

/*----------------------------------------------------------------------------*/

} // namespace vq::tests::core::ResourceManager
