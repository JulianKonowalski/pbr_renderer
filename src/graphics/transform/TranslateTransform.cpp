#include "graphics/transform/TranslateTransform.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace vq::graphics::transform;

/*----------------------------------------------------------------------------*/

TranslateTransform::TranslateTransform()
    : TransformBase(), m_translation(0.0f, 0.0f, 0.0f) {}

/*----------------------------------------------------------------------------*/

TranslateTransform::TranslateTransform(TranslateTransform&& other)
    : TransformBase(std::move(other)),
      m_translation(std::move(other.m_translation)) {}

/*----------------------------------------------------------------------------*/

TranslateTransform::TranslateTransform(const TranslateTransform& other)
    : TransformBase(other), m_translation(other.m_translation) {}

/*----------------------------------------------------------------------------*/

TranslateTransform& TranslateTransform::operator=(TranslateTransform&& other) {
    m_translation = std::move(other.m_translation);
    TransformBase::operator=(std::move(other));
    return *this;
}

/*----------------------------------------------------------------------------*/

TranslateTransform&
TranslateTransform::operator=(const TranslateTransform& other) {
    m_translation = other.m_translation;
    TransformBase::operator=(other);
    return *this;
}

/*----------------------------------------------------------------------------*/

void TranslateTransform::on_reset() {
    m_translation = glm::vec3(0.0f, 0.0f, 0.0f);
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void TranslateTransform::translate(const glm::vec3& translation) {
    for (int i = 0; i < 3; ++i) {
        m_translation[i] += translation[i];
    }
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void TranslateTransform::set_translation(const glm::vec3& translation) {
    memcpy(&m_translation[0], &translation[0], sizeof(float) * 3);
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void TranslateTransform::on_update() {
    m_transform_matrix = glm::translate(glm::mat4(1.0f), m_translation);
}

/*----------------------------------------------------------------------------*/
