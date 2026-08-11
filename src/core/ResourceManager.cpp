#include "core/ResourceManager.hpp"

using namespace vq::core;

/*----------------------------------------------------------------------------*/

ResourceManager& ResourceManager::get_instance() {
    static ResourceManager resource_manager;
    return resource_manager;
}

/*----------------------------------------------------------------------------*/
