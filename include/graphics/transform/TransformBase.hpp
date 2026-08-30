#pragma once

#include <glm/mat4x4.hpp>

namespace vq::graphics::transform {

/*----------------------------------------------------------------------------*/

class TransformBase {
  public:
    TransformBase();
    ~TransformBase() = default;

    TransformBase(TransformBase&& other);
    TransformBase(const TransformBase& other);

    TransformBase& operator=(TransformBase&& other);
    TransformBase& operator=(const TransformBase& other);

    inline bool is_dirty() const;

    void reset_transform();
    void update_transform_matrix();

    const glm::mat4& get_transform_matrix();

  protected:
    virtual void on_reset()  = 0;
    virtual void on_update() = 0;

    inline void set_dirty();

    glm::mat4 m_transform_matrix;

  private:
    bool m_is_dirty;
};

/*----------------------------------------------------------------------------*/

inline bool TransformBase::is_dirty() const { return m_is_dirty; }

/*----------------------------------------------------------------------------*/

inline void TransformBase::set_dirty() { m_is_dirty = true; }

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::transform
