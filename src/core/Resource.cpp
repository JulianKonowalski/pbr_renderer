#include "core/Resource.hpp"

using namespace vq::core;

/*----------------------------------------------------------------------------*/

Resource::Resource(const std::string& id) : m_is_loaded(false), m_id(id) {}

/*----------------------------------------------------------------------------*/

Resource::Resource(Resource&& other) { *this = std::move(other); }

/*----------------------------------------------------------------------------*/

Resource& Resource::operator=(Resource&& other) {
    m_id        = std::move(other.m_id);
    m_is_loaded = other.m_is_loaded;
    return *this;
}

/*----------------------------------------------------------------------------*/

bool Resource::load() {
    m_is_loaded = this->do_load();
    return m_is_loaded;
}

/*----------------------------------------------------------------------------*/

void Resource::unload() {
    if (!m_is_loaded) {
        return;
    }
    this->do_unload();
    m_is_loaded = false;
}

/*----------------------------------------------------------------------------*/
