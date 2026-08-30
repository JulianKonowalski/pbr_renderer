#pragma once

#include "graphics/transform/TransformBase.hpp"

#include <glm/vec3.hpp>

namespace vq::graphics::transform {

/*----------------------------------------------------------------------------*/

class TranslateTransform final : public TransformBase {
  public:
    TranslateTransform();
    ~TranslateTransform() = default;

    TranslateTransform(TranslateTransform&& other);
    TranslateTransform(const TranslateTransform& other);

    TranslateTransform& operator=(TranslateTransform&& other);
    TranslateTransform& operator=(const TranslateTransform& other);

    inline const glm::vec3& get_translation() const;

    void translate(const glm::vec3& translation);
    void set_translation(const glm::vec3& translation);

  protected:
    void on_reset() override;
    void on_update() override;

  private:
    glm::vec3 m_translation;
};

/*----------------------------------------------------------------------------*/

inline const glm::vec3& TranslateTransform::get_translation() const {
    return m_translation;
}

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::transform
