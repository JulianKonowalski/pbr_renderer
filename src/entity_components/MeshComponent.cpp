#include "entity_components/MeshComponent.hpp"

#include <glad/glad.h>

using namespace vq::entity_components;
using namespace vq::graphics::core;

/*----------------------------------------------------------------------------*/

MeshComponent::MeshComponent(vq::core::Entity& parent, Geometry* geometry)
    : vq::core::Component(parent), m_geometry(geometry) {}

/*----------------------------------------------------------------------------*/

MeshComponent::~MeshComponent() { delete m_geometry; }

/*----------------------------------------------------------------------------*/

void MeshComponent::render() {
    /** TODO: implement render commands */
    glBindVertexArray(m_geometry->m_vertex_array);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 m_geometry->m_buffers[Geometry::AttributeType::INDEX]);
    glDrawElements(GL_LINE_LOOP,
                   m_geometry->m_buffer_sizes[Geometry::AttributeType::INDEX],
                   GL_UNSIGNED_INT, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);
}

/*----------------------------------------------------------------------------*/
