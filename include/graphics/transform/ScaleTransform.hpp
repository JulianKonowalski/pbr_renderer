#pragma once

#include "graphics/transform/TransformBase.hpp"

#include <glm/vec3.hpp>

namespace vq::graphics::transform {

/*----------------------------------------------------------------------------*/

class ScaleTransform final : public TransformBase {
  public:
    ScaleTransform();
    ~ScaleTransform() = default;

    ScaleTransform(ScaleTransform&& other);
    ScaleTransform(const ScaleTransform& other);

    ScaleTransform& operator=(ScaleTransform&& other);
    ScaleTransform& operator=(const ScaleTransform& other);

    inline const glm::vec3& get_scale() const;

    void scale(const glm::vec3& scale);
    void set_scale(const glm::vec3& scale);

  protected:
    void on_reset() override;
    void on_update() override;

  private:
    glm::vec3 m_scale;
};

/*----------------------------------------------------------------------------*/

inline const glm::vec3& ScaleTransform::get_scale() const { return m_scale; }

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::transform
