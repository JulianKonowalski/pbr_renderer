#include "graphics/core/Geometry.hpp"

#include <glad/glad.h>
#include <stdexcept>

using namespace vq::graphics::core;

Geometry Geometry::box_geometry(const std::array<float, 3>& min_vertex,
                                const std::array<float, 3>& max_vertex,
                                const std::string& id) {
    Geometry geometry(id);
    return std::move(geometry);
}

/*----------------------------------------------------------------------------*/

Geometry Geometry::plane_geometry(const std::array<float, 2>& min_vertex,
                                  const std::array<float, 2>& max_vertex,
                                  const std::string& id) {
    Geometry geometry(id);

    glBindVertexArray(geometry.m_vertex_array);

    {
        std::array<float, 12> positions          = {0.0f};
        std::array<float, 12> normals            = {0.0f};
        std::array<float, 8> texture_coordinates = {0.0f};

        glBindBuffer(GL_ARRAY_BUFFER,
                     geometry.m_buffers[AttributeType::POSITION]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, positions.data(),
                     GL_STATIC_DRAW);
        geometry.m_buffer_sizes[AttributeType::POSITION] = 12;

        glBindBuffer(GL_ARRAY_BUFFER,
                     geometry.m_buffers[AttributeType::NORMAL]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, normals.data(),
                     GL_STATIC_DRAW);
        geometry.m_buffer_sizes[AttributeType::NORMAL] = 12;

        glBindBuffer(GL_ARRAY_BUFFER,
                     geometry.m_buffers[AttributeType::TEXTURE_COORDINATES]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8,
                     texture_coordinates.data(), GL_STATIC_DRAW);
        geometry.m_buffer_sizes[AttributeType::TEXTURE_COORDINATES] = 8;

        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    }
    {
        std::array<unsigned int, 6> indices = {};

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                     geometry.m_buffers[AttributeType::INDEX]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6,
                     indices.data(), GL_STATIC_DRAW);
        geometry.m_buffer_sizes[AttributeType::INDEX] = 6;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
    }

    glBindVertexArray(GL_NONE);

    return std::move(geometry);
}

/*----------------------------------------------------------------------------*/

Geometry Geometry::sphere_geometry(const float radius,
                                   const unsigned int num_stacks,
                                   const unsigned int num_sectors,
                                   const std::string& id) {
    Geometry geometry(id);
    return std::move(geometry);
}

/*----------------------------------------------------------------------------*/

Geometry::Geometry(const std::string& id, const std::string& asset_path)
    : m_vertex_array(0), m_buffers({0}), m_buffer_sizes({0}),
      vq::core::Resource(id, asset_path) {
    glGenVertexArrays(1, &m_vertex_array);
    if (!m_vertex_array) {
        throw std::runtime_error(
            "Failed to create a vertex array for geometry " + id +
            " during a call to " + __FUNCTION__);
    }
    glBindVertexArray(m_vertex_array);

    glGenBuffers(m_buffers.size(), m_buffers.data());
    for (const auto& buffer : m_buffers) {
        if (!buffer) {
            throw std::runtime_error("Failed to create a buffer for geometry " +
                                     id + " during a call to " + __FUNCTION__);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[AttributeType::POSITION]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[AttributeType::NORMAL]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER,
                 m_buffers[AttributeType::TEXTURE_COORDINATES]);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);
}

/*----------------------------------------------------------------------------*/

Geometry::~Geometry() {
    this->unload();
    glDeleteBuffers(AttributeType::COUNT, m_buffers.data());
    glDeleteVertexArrays(1, &m_vertex_array);
}

/*----------------------------------------------------------------------------*/

Geometry::Geometry(Geometry&& other) : vq::core::Resource("") {
    *this = std::move(other);
}

/*----------------------------------------------------------------------------*/

Geometry& Geometry::operator=(Geometry&& other) {
    m_buffers      = std::move(other.m_buffers);
    m_buffer_sizes = std::move(other.m_buffer_sizes);
    m_vertex_array = other.m_vertex_array;
    vq::core::Resource::operator=(std::move(other));

    /**
     * This HAS to be set to 0, otherwise one object will deallocate and
     * invalidate the GPU resources that the second one wants to keep using
     */
    other.m_vertex_array = 0;
    for (int i = 0; i < AttributeType::COUNT; ++i) {
        other.m_buffers[i]      = 0;
        other.m_buffer_sizes[i] = 0;
    }

    return *this;
}

/*----------------------------------------------------------------------------*/

bool Geometry::do_load() noexcept {
    if (this->get_asset_path() == "") {
        return true;
    }
    /**
     * TODO: read file & load data here
     */
    return true;
}

/*----------------------------------------------------------------------------*/

void Geometry::do_unload() noexcept { /** TODO: unload data here */ }

/*----------------------------------------------------------------------------*/
