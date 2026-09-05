#include "graphics/core/Texture.hpp"

using namespace vq::graphics::core;

/*----------------------------------------------------------------------------*/

Texture::Texture(const std::string& id, const std::string& source_file)
    : Resource(id), m_source_file(source_file), m_texture_id(0) {}

/*----------------------------------------------------------------------------*/

Texture::Texture(Texture&& other)
    : Resource(std::move(other)), m_source_file(std::move(other.m_source_file)),
      m_texture_id(other.m_texture_id) {
    other.m_texture_id = 0;
}

/*----------------------------------------------------------------------------*/

Texture& Texture::operator=(Texture&& other) {
    m_source_file      = std::move(other.m_source_file);
    m_texture_id       = other.m_texture_id;
    other.m_texture_id = 0;
    Resource::operator=(std::move(other));
    return *this;
}

/*----------------------------------------------------------------------------*/
