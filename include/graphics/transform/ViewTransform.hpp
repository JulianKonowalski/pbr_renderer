#pragma once

#include <graphics/transform/RotateTransform.hpp>
#include <graphics/transform/TransformBase.hpp>
#include <graphics/transform/TranslateTransform.hpp>

namespace vq::graphics::transform {

/*----------------------------------------------------------------------------*/

class ViewTransform final
    : public TransformBase,
      public vq::core::EventHandler<TransformChangeEvent> {
  public:
    explicit ViewTransform(RotateTransform& rotate_transform,
                           TranslateTransform& translate_transform);
    ~ViewTransform() = default;

    ViewTransform(ViewTransform&& other);
    ViewTransform(const ViewTransform& other);

    ViewTransform& operator=(ViewTransform&& other);
    ViewTransform& operator=(const ViewTransform& other);

    inline glm::vec3 get_up_vector() const;
    inline glm::vec3 get_front_vector() const;
    inline glm::vec3 get_right_vector() const;

    inline RotateTransform& get_rotate_transform();
    inline TranslateTransform& get_translate_transform();

    inline const RotateTransform& get_rotate_transform() const;
    inline const TranslateTransform& get_translate_transform() const;

    void set_rotate_transform(RotateTransform& rotate_transform);
    void set_translate_transform(TranslateTransform& translate_transform);

    void handle(TransformChangeEvent& event) noexcept override;

  protected:
    void on_reset() override;
    void on_update() override;

  private:
    RotateTransform* m_rotate_transform;
    TranslateTransform* m_translate_transform;
};

/*----------------------------------------------------------------------------*/

inline glm::vec3 ViewTransform::get_up_vector() const {
    return glm::normalize(TransformBase::s_world_up *
                          m_rotate_transform->get_rotation_quaternion());
}

inline glm::vec3 ViewTransform::get_front_vector() const {
    return glm::normalize(TransformBase::s_world_front *
                          m_rotate_transform->get_rotation_quaternion());
}

inline glm::vec3 ViewTransform::get_right_vector() const {
    return glm::normalize(TransformBase::s_world_right *
                          m_rotate_transform->get_rotation_quaternion());
}

inline RotateTransform& ViewTransform::get_rotate_transform() {
    return *m_rotate_transform;
}

inline TranslateTransform& ViewTransform::get_translate_transform() {
    return *m_translate_transform;
}

inline const RotateTransform& ViewTransform::get_rotate_transform() const {
    return *m_rotate_transform;
}

inline const TranslateTransform&
ViewTransform::get_translate_transform() const {
    return *m_translate_transform;
}

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::transform
