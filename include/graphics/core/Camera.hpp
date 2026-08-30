#pragma once

#include <array>

namespace vq::graphics::core {

class Camera {
  public:
    const std::array<float, 3>& get_vector_up() const;
    const std::array<float, 3>& get_vector_front() const;
    const std::array<float, 3>& get_vector_right() const;
    const std::array<float, 16>& get_view_matrix();

    virtual const std::array<float, 16>& get_view_projection_matrix() = 0;

  protected:
    std::array<float, 16> m_projection_matrix;
    bool m_projection_matrix_dirty;

  private:
    std::array<float, 3> m_vector_up;
    std::array<float, 3> m_vector_front;
    std::array<float, 3> m_vector_right;

    std::array<float, 16> m_view_matrix;
    bool m_view_matrix_dirty;
};

} // namespace vq::graphics::core
