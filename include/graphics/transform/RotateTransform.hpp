#pragma once

#include "graphics/transform/TransformBase.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace vq::graphics::transform {

/*----------------------------------------------------------------------------*/

class RotateTransform final : public TransformBase {
  public:
    RotateTransform();
    ~RotateTransform() = default;

    RotateTransform(RotateTransform&& other);
    RotateTransform(const RotateTransform& other);

    RotateTransform& operator=(RotateTransform&& other);
    RotateTransform& operator=(const RotateTransform& other);

    inline const glm::quat& get_rotation_quaternion() const;
    glm::vec3 get_rotation_euler_radians() const;
    glm::vec3 get_rotation_euler_degrees() const;

    void rotate_quaternion(const glm::quat& rotation);
    void rotate_euler_radians(const glm::vec3& rotation);
    void rotate_euler_degrees(const glm::vec3& rotation);
    void rotate_on_axis_radians(const float angle, const glm::vec3& axis);
    void rotate_on_axis_degrees(const float angle, const glm::vec3& axis);

    void set_rotation_quaternion(const glm::quat& rotation);
    void set_rotation_euler_radians(const glm::vec3& rotation);
    void set_rotation_euler_degrees(const glm::vec3& rotation);

  protected:
    void on_reset() override;
    void on_update() override;

  private:
    glm::quat m_rotation;
};

/*----------------------------------------------------------------------------*/

inline const glm::quat& RotateTransform::get_rotation_quaternion() const {
    return m_rotation;
}

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::transform
