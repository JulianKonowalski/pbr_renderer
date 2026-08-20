#include "graphics/core/Geometry.hpp"

#include <glad/glad.h>
#include <iostream>

using namespace vq::graphics::core;

/*----------------------------------------------------------------------------*/

Geometry::Geometry(const std::string& id, const std::string& source_file_path)
    : vq::core::Resource(id), m_source_file(source_file_path),
      m_attribute_sizes({0}), m_indices_size(0), m_attributes({0}),
      m_vertex_array(0), m_index_array(0) {}

/*----------------------------------------------------------------------------*/

Geometry::Geometry(Geometry&& other) : vq::core::Resource("") {
    *this = std::move(other);
}

/*----------------------------------------------------------------------------*/

Geometry& Geometry::operator=(Geometry&& other) {
    m_source_file = std::move(other.m_source_file);
    for (int i = 0; i < m_attributes.size(); ++i) {
        m_attributes[i]       = other.m_attributes[i];
        other.m_attributes[i] = 0;
    }
    m_vertex_array       = other.m_vertex_array;
    m_index_array        = other.m_index_array;
    other.m_vertex_array = 0;
    other.m_index_array  = 0;
    vq::core::Resource::operator=(std::move(other));
    return *this;
}

/*----------------------------------------------------------------------------*/

void Geometry::unbind_all() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);
}

/*----------------------------------------------------------------------------*/

void Geometry::get_indices(std::vector<unsigned int>& data) const {
    int index_buffer_size;
    glGetBufferParameteriv(m_index_array, GL_BUFFER_SIZE, &index_buffer_size);
    data.resize(index_buffer_size);

    this->bind();
    glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, index_buffer_size,
                       data.data());
    this->unbind();
}

/*----------------------------------------------------------------------------*/

void Geometry::set_indices(const std::vector<unsigned int>& data) {
    this->bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int),
                 data.data(), GL_STATIC_DRAW);
    this->unbind();
    m_indices_size = data.size();
}

/*----------------------------------------------------------------------------*/

void Geometry::bind() const {
    glBindVertexArray(m_vertex_array);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_array);
}

/*----------------------------------------------------------------------------*/

void Geometry::unbind() const {
    int current_vertex_array;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vertex_array);
    if (current_vertex_array != m_vertex_array) {
        return;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);
}

/*----------------------------------------------------------------------------*/

bool Geometry::do_load() noexcept {
    glCreateVertexArrays(1, &m_vertex_array);
    if (!m_vertex_array) {
        std::cout << "Failed to create a Vertex Array\n";
        return false;
    }

    glBindVertexArray(m_vertex_array);
    glCreateBuffers(1, &m_index_array);
    if (!m_index_array) {
        std::cout << "Failed to create an Index Buffer\n";
        this->do_unload();
        return false;
    }

    glCreateBuffers(m_attributes.size(), m_attributes.data());
    for (auto attribute : m_attributes) {
        if (!attribute) {
            std::cout << "Failed to create one of the Vertex Buffers\n";
            this->do_unload();
            return false;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER,
                 m_attributes[static_cast<uint8_t>(Attribute::POSITION)]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER,
                 m_attributes[static_cast<uint8_t>(Attribute::NORMAL)]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_attributes[static_cast<uint8_t>(Attribute::TEXTURE_COORDINATE)]);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);

    if (m_source_file.empty()) {
        return true;
    }

    // load source file data

    return true;
}

/*----------------------------------------------------------------------------*/

bool Geometry::do_reload() noexcept {
    if (m_source_file.empty()) {
        return true;
    }

    // load source file data

    return false;
}

/*----------------------------------------------------------------------------*/

void Geometry::do_unload() noexcept {
    this->unbind();

    glDeleteVertexArrays(1, &m_vertex_array);
    glDeleteBuffers(1, &m_index_array);
    glDeleteBuffers(m_attributes.size(), m_attributes.data());

    m_vertex_array = 0;
    m_index_array  = 0;
    for (auto& attribute : m_attributes) {
        attribute = 0;
    }
}

/*----------------------------------------------------------------------------*/

void Geometry::download_attribute_data(std::vector<float>& data,
                                       Attribute attribute) const {
    int attribute_buffer_size;
    glGetBufferParameteriv(m_attributes[static_cast<uint8_t>(attribute)],
                           GL_BUFFER_SIZE, &attribute_buffer_size);
    data.resize(attribute_buffer_size);

    this->bind();
    glBindBuffer(GL_ARRAY_BUFFER,
                 m_attributes[static_cast<uint8_t>(attribute)]);
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, attribute_buffer_size, data.data());
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    this->unbind();
}

/*----------------------------------------------------------------------------*/

void Geometry::upload_attribute_data(const std::vector<float>& data,
                                     Attribute attribute) {
    this->bind();
    glBindBuffer(GL_ARRAY_BUFFER,
                 m_attributes[static_cast<uint8_t>(attribute)]);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    this->unbind();
}

/*----------------------------------------------------------------------------*/
