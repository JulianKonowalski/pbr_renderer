#include "graphics/transform/RotateTransform.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace vq::graphics::transform;

/*----------------------------------------------------------------------------*/

/**
 * I know that C++ 20 standard defines the value of PI, but for some reason
 * sometimes the compiler thinks I'm not using C++ 20 and M_PI from cmath also
 * doesn't compile under all conditions. It's one constant, so I just define it
 * here, inline
 */
static constexpr float pi2 = 2.0f * 3.14159265358979323846;

/*----------------------------------------------------------------------------*/

RotateTransform::RotateTransform()
    : TransformBase(), m_rotation(1.0f, 0.0f, 0.0f, 0.0f) {}

/*----------------------------------------------------------------------------*/

RotateTransform::RotateTransform(RotateTransform&& other)
    : TransformBase(std::move(other)), m_rotation(std::move(other.m_rotation)) {
}

/*----------------------------------------------------------------------------*/

RotateTransform::RotateTransform(const RotateTransform& other)
    : TransformBase(other), m_rotation(other.m_rotation) {}

/*----------------------------------------------------------------------------*/

RotateTransform& RotateTransform::operator=(RotateTransform&& other) {
    m_rotation = std::move(other.m_rotation);
    TransformBase::operator=(std::move(other));
    return *this;
}

/*----------------------------------------------------------------------------*/

RotateTransform& RotateTransform::operator=(const RotateTransform& other) {
    m_rotation = other.m_rotation;
    TransformBase::operator=(other);
    return *this;
}

/*----------------------------------------------------------------------------*/

glm::vec3 RotateTransform::get_rotation_euler_radians() const {
    return glm::eulerAngles(m_rotation);
}

/*----------------------------------------------------------------------------*/

glm::vec3 RotateTransform::get_rotation_euler_degrees() const {
    glm::vec3 rotation = glm::eulerAngles(m_rotation);
    for (int i = 0; i < 3; ++i) {
        rotation[i] = glm::degrees(rotation[i]);
    }
    return rotation;
}

/*----------------------------------------------------------------------------*/

void RotateTransform::rotate_quaternion(const glm::quat& rotation) {
    m_rotation = m_rotation * rotation;
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void RotateTransform::rotate_euler_radians(const glm::vec3& rotation) {
    glm::quat quaternion_rotation =
        glm::angleAxis(std::fmod(rotation[0], pi2), glm::vec3(1.0, 0.0, 0.0)) *
        glm::angleAxis(std::fmod(rotation[1], pi2), glm::vec3(0.0, 1.0, 0.0)) *
        glm::angleAxis(std::fmod(rotation[2], pi2), glm::vec3(0.0, 0.0, 1.0));
    m_rotation = m_rotation * quaternion_rotation;
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void RotateTransform::rotate_euler_degrees(const glm::vec3& rotation) {
    glm::quat quaternion_rotation =
        glm::angleAxis(glm::radians(std::fmod(rotation[0], 360.0f)),
                       glm::vec3(1.0, 0.0, 0.0)) *
        glm::angleAxis(glm::radians(std::fmod(rotation[1], 360.0f)),
                       glm::vec3(0.0, 1.0, 0.0)) *
        glm::angleAxis(glm::radians(std::fmod(rotation[2], 360.0f)),
                       glm::vec3(0.0, 0.0, 1.0));
    m_rotation = m_rotation * quaternion_rotation;
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void RotateTransform::rotate_on_axis_radians(float angle,
                                             const glm::vec3& axis) {
    glm::quat quaternion_rotation = glm::angleAxis(std::fmod(angle, pi2), axis);
    m_rotation                    = m_rotation * quaternion_rotation;
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void RotateTransform::rotate_on_axis_degrees(float angle,
                                             const glm::vec3& axis) {
    glm::quat quaternion_rotation =
        glm::angleAxis(glm::radians(std::fmod(angle, 360.0f)), axis);
    m_rotation = m_rotation * quaternion_rotation;
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void RotateTransform::set_rotation_quaternion(const glm::quat& rotation) {
    m_rotation = rotation;
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void RotateTransform::set_rotation_euler_radians(const glm::vec3& rotation) {
    m_rotation =
        glm::angleAxis(std::fmod(rotation[0], pi2), glm::vec3(1.0, 0.0, 0.0)) *
        glm::angleAxis(std::fmod(rotation[1], pi2), glm::vec3(0.0, 1.0, 0.0)) *
        glm::angleAxis(std::fmod(rotation[2], pi2), glm::vec3(0.0, 0.0, 1.0));
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void RotateTransform::set_rotation_euler_degrees(const glm::vec3& rotation) {
    m_rotation = glm::angleAxis(glm::radians(std::fmod(rotation[0], 360.0f)),
                                glm::vec3(1.0, 0.0, 0.0)) *
                 glm::angleAxis(glm::radians(std::fmod(rotation[1], 360.0f)),
                                glm::vec3(0.0, 1.0, 0.0)) *
                 glm::angleAxis(glm::radians(std::fmod(rotation[2], 360.0f)),
                                glm::vec3(0.0, 0.0, 1.0));
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void RotateTransform::on_reset() { m_rotation = {1.0f, 0.0f, 0.0f, 0.0f}; }

/*----------------------------------------------------------------------------*/

void RotateTransform::on_update() {
    m_transform_matrix = glm::mat4_cast(m_rotation);
}

/*----------------------------------------------------------------------------*/
