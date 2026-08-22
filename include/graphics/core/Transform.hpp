#pragma once

#include <array>

namespace vq::graphics::core {

/*----------------------------------------------------------------------------*/

class Transform final {
  public:
    Transform();
    ~Transform() = default;

    Transform(Transform&& other);
    Transform(const Transform& other);

    Transform& operator=(Transform&& other);
    Transform& operator=(const Transform& other);

    inline const std::array<float, 4>& get_rotation_quaternion() const;
    inline const std::array<float, 3>& get_scale() const;
    inline const std::array<float, 3>& get_translation() const;

    std::array<float, 3> get_rotation_euler_radians() const;
    std::array<float, 3> get_rotation_euler_degrees() const;
    const std::array<float, 16>& get_transform_matrix();

    void reset();

    void set_rotation_quaternion(const std::array<float, 4>& quaternion);
    void set_rotation_euler_radians(const std::array<float, 3>& rotation);
    void set_rotation_euler_degrees(const std::array<float, 3>& rotation);
    void set_scale(const std::array<float, 3>& scale);
    void set_translation(const std::array<float, 3>& translation);

    void rotate_quaternion(const std::array<float, 4>& quaternion);
    void rotate_euler_radians(const std::array<float, 3>& rotation);
    void rotate_euler_degrees(const std::array<float, 3>& rotation);
    void scale(const std::array<float, 3>& scale);
    void translate(const std::array<float, 3>& translation);

  private:
    std::array<float, 16> m_transform_matrix;
    std::array<float, 4> m_rotation;
    std::array<float, 3> m_scale;
    std::array<float, 3> m_translation;
    bool m_is_dirty;
};

/*----------------------------------------------------------------------------*/

inline const std::array<float, 4>& Transform::get_rotation_quaternion() const {
    return m_rotation;
}

inline const std::array<float, 3>& Transform::get_scale() const {
    return m_scale;
}

inline const std::array<float, 3>& Transform::get_translation() const {
    return m_translation;
}

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::core
