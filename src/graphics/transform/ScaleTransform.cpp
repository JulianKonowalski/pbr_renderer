#include "graphics/transform/ScaleTransform.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace vq::graphics::transform;

/*----------------------------------------------------------------------------*/

ScaleTransform::ScaleTransform() : TransformBase(), m_scale(1.0f, 1.0f, 1.0f) {}

/*----------------------------------------------------------------------------*/

ScaleTransform::ScaleTransform(ScaleTransform&& other)
    : TransformBase(std::move(other)), m_scale(std::move(other.m_scale)) {}

/*----------------------------------------------------------------------------*/

ScaleTransform::ScaleTransform(const ScaleTransform& other)
    : TransformBase(other), m_scale(other.m_scale) {}

/*----------------------------------------------------------------------------*/

ScaleTransform& ScaleTransform::operator=(ScaleTransform&& other) {
    m_scale = std::move(other.m_scale);
    TransformBase::operator=(std::move(other));
    return *this;
}

/*----------------------------------------------------------------------------*/

ScaleTransform& ScaleTransform::operator=(const ScaleTransform& other) {
    m_scale = other.m_scale;
    TransformBase::operator=(other);
    return *this;
}

/*----------------------------------------------------------------------------*/

void ScaleTransform::on_reset() {
    m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void ScaleTransform::scale(const glm::vec3& scale) {
    for (int i = 0; i < 3; ++i) {
        m_scale[i] *= scale[i];
    }
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void ScaleTransform::set_scale(const glm::vec3& scale) {
    memcpy(&m_scale[0], &scale[0], sizeof(float) * 3);
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void ScaleTransform::on_update() {
    m_transform_matrix = glm::scale(glm::mat4x4(1.0f), m_scale);
}

/*----------------------------------------------------------------------------*/
