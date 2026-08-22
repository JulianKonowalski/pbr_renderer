#include "graphics/core/Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

using namespace vq::graphics::core;

/*----------------------------------------------------------------------------*/

/**
 * I know that C++ 20 standard defines the value of PI, but for some reason
 * sometimes the compiler thinks I'm not using C++ 20 and M_PI from cmath also
 * doesn't compile under all conditions. It's one constant, so I just define it
 * here, inline
 */
static constexpr float pi2 = 2.0f * 3.14159265358979323846;

/*----------------------------------------------------------------------------*/

Transform::Transform()
    : m_transform_matrix({0.0f}), m_rotation({1.0f, 0.0f, 0.0f, 0.0f}),
      m_scale({1.0f, 1.0f, 1.0f}), m_translation({0.0f, 0.0f, 0.0f}),
      m_is_dirty(true) {}

/*----------------------------------------------------------------------------*/

Transform::Transform(Transform&& other) { *this = std::move(other); }

/*----------------------------------------------------------------------------*/

Transform::Transform(const Transform& other) { *this = other; }

/*----------------------------------------------------------------------------*/
Transform& Transform::operator=(Transform&& other) {
    m_transform_matrix = std::move(other.m_transform_matrix);
    m_rotation         = std::move(other.m_rotation);
    m_scale            = std::move(other.m_scale);
    m_translation      = std::move(other.m_translation);
    m_is_dirty         = other.m_is_dirty;
    return *this;
}

/*----------------------------------------------------------------------------*/

Transform& Transform::operator=(const Transform& other) {
    m_transform_matrix = other.m_transform_matrix;
    m_rotation         = other.m_rotation;
    m_scale            = other.m_scale;
    m_translation      = other.m_translation;
    m_is_dirty         = other.m_is_dirty;
    return *this;
}

/*----------------------------------------------------------------------------*/

std::array<float, 3> Transform::get_rotation_euler_radians() const {
    glm::vec3 rotation = glm::eulerAngles(
        glm::quat(m_rotation[0], m_rotation[1], m_rotation[2], m_rotation[3]));
    return std::array<float, 3>({rotation[0], rotation[1], rotation[2]});
}

/*----------------------------------------------------------------------------*/

std::array<float, 3> Transform::get_rotation_euler_degrees() const {
    glm::vec3 rotation = glm::eulerAngles(
        glm::quat(m_rotation[0], m_rotation[1], m_rotation[2], m_rotation[3]));
    return std::array<float, 3>({glm::degrees(rotation[0]),
                                 glm::degrees(rotation[1]),
                                 glm::degrees(rotation[2])});
}

/*----------------------------------------------------------------------------*/

const std::array<float, 16>& Transform::get_transform_matrix() {
    if (m_is_dirty) {
        glm::mat4x4 transform_matrix =
            glm::translate(glm::mat4x4(1.0f),
                           glm::vec3(m_translation[0], m_translation[1],
                                     m_translation[2])) *
            glm::mat4_cast(glm::quat(m_rotation[0], m_rotation[1],
                                     m_rotation[2], m_rotation[3])) *
            glm::scale(glm::mat4x4(1.0f),
                       glm::vec3(m_scale[0], m_scale[1], m_scale[2]));
        memcpy(m_transform_matrix.data(), &transform_matrix[0][0],
               sizeof(float) * 16);
        m_is_dirty = false;
    }
    return m_transform_matrix;
}

/*----------------------------------------------------------------------------*/

void Transform::reset() {
    m_rotation    = {1.0, 0.0, 0.0, 0.0};
    m_scale       = {1.0, 1.0, 1.0};
    m_translation = {0.0, 0.0, 0.0};
    m_is_dirty    = true;
}

/*----------------------------------------------------------------------------*/

void Transform::set_rotation_quaternion(
    const std::array<float, 4>& quaternion) {
    m_rotation = quaternion;
    m_is_dirty = true;
}

/*----------------------------------------------------------------------------*/

void Transform::set_rotation_euler_radians(
    const std::array<float, 3>& rotation) {
    glm::quat result =
        glm::angleAxis(std::fmod(rotation[0], pi2), glm::vec3(1.0, 0.0, 0.0)) *
        glm::angleAxis(std::fmod(rotation[1], pi2), glm::vec3(0.0, 1.0, 0.0)) *
        glm::angleAxis(std::fmod(rotation[2], pi2), glm::vec3(0.0, 0.0, 1.0));
    m_rotation = {result.w, result.x, result.y, result.z};
    m_is_dirty = true;
}

/*----------------------------------------------------------------------------*/

void Transform::set_rotation_euler_degrees(
    const std::array<float, 3>& rotation) {
    glm::quat result =
        glm::angleAxis(glm::radians(std::fmod(rotation[0], 360.0f)),
                       glm::vec3(1.0, 0.0, 0.0)) *
        glm::angleAxis(glm::radians(std::fmod(rotation[1], 360.0f)),
                       glm::vec3(0.0, 1.0, 0.0)) *
        glm::angleAxis(glm::radians(std::fmod(rotation[2], 360.0f)),
                       glm::vec3(0.0, 0.0, 1.0));
    m_rotation = {result.w, result.x, result.y, result.z};
    m_is_dirty = true;
}

/*----------------------------------------------------------------------------*/

void Transform::set_scale(const std::array<float, 3>& scale) {
    m_scale    = scale;
    m_is_dirty = true;
}

/*----------------------------------------------------------------------------*/

void Transform::set_translation(const std::array<float, 3>& translation) {
    m_translation = translation;
    m_is_dirty    = true;
}

/*----------------------------------------------------------------------------*/

void Transform::rotate_quaternion(const std::array<float, 4>& quaternion) {
    glm::quat lhs(m_rotation[0], m_rotation[1], m_rotation[2], m_rotation[3]);
    glm::quat rhs(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
    glm::quat result = lhs * rhs;
    m_rotation       = {result.w, result.x, result.y, result.z};
    m_is_dirty       = true;
}

/*----------------------------------------------------------------------------*/

void Transform::rotate_euler_radians(const std::array<float, 3>& rotation) {
    glm::quat lhs(m_rotation[0], m_rotation[1], m_rotation[2], m_rotation[3]);
    glm::quat rhs =
        glm::angleAxis(std::fmod(rotation[0], pi2), glm::vec3(1.0, 0.0, 0.0)) *
        glm::angleAxis(std::fmod(rotation[1], pi2), glm::vec3(0.0, 1.0, 0.0)) *
        glm::angleAxis(std::fmod(rotation[2], pi2), glm::vec3(0.0, 0.0, 1.0));
    glm::quat result = lhs * rhs;
    m_rotation       = {result.w, result.x, result.y, result.z};
    m_is_dirty       = true;
}

/*----------------------------------------------------------------------------*/

void Transform::rotate_euler_degrees(const std::array<float, 3>& rotation) {
    glm::quat lhs(m_rotation[0], m_rotation[1], m_rotation[2], m_rotation[3]);
    glm::quat rhs = glm::angleAxis(glm::radians(std::fmod(rotation[0], 360.0f)),
                                   glm::vec3(1.0, 0.0, 0.0)) *
                    glm::angleAxis(glm::radians(std::fmod(rotation[1], 360.0f)),
                                   glm::vec3(0.0, 1.0, 0.0)) *
                    glm::angleAxis(glm::radians(std::fmod(rotation[2], 360.0f)),
                                   glm::vec3(0.0, 0.0, 1.0));
    glm::quat result = lhs * rhs;
    m_rotation       = {result.w, result.x, result.y, result.z};
    m_is_dirty       = true;
}

/*----------------------------------------------------------------------------*/

void Transform::scale(const std::array<float, 3>& scale) {
    for (int i = 0; i < m_scale.size(); ++i) {
        m_scale[i] *= scale[i];
    }
    m_is_dirty = true;
}

/*----------------------------------------------------------------------------*/

void Transform::translate(const std::array<float, 3>& translation) {
    for (int i = 0; i < m_translation.size(); ++i) {
        m_translation[i] += translation[i];
    }
    m_is_dirty = true;
}

/*----------------------------------------------------------------------------*/
