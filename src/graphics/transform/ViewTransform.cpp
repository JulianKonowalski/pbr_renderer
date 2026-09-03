#include "graphics/transform/ViewTransform.hpp"

using namespace vq::graphics::transform;

/*----------------------------------------------------------------------------*/

ViewTransform::ViewTransform(RotateTransform& rotate_transform,
                             TranslateTransform& translate_transform)
    : m_rotate_transform(&rotate_transform),
      m_translate_transform(&translate_transform) {
    m_rotate_transform->attach_event_handler(*this);
    m_translate_transform->attach_event_handler(*this);
}

/*----------------------------------------------------------------------------*/

ViewTransform::ViewTransform(ViewTransform&& other)
    : EventHandler(std::move(other)), TransformBase(std::move(other)),
      m_rotate_transform(other.m_rotate_transform),
      m_translate_transform(other.m_translate_transform) {}

/*----------------------------------------------------------------------------*/

ViewTransform::ViewTransform(const ViewTransform& other)
    : EventHandler(other), TransformBase(other),
      m_rotate_transform(other.m_rotate_transform),
      m_translate_transform(other.m_translate_transform) {}

/*----------------------------------------------------------------------------*/

ViewTransform& ViewTransform::operator=(ViewTransform&& other) {
    m_rotate_transform    = other.m_rotate_transform;
    m_translate_transform = other.m_translate_transform;
    TransformBase::operator=(std::move(other));
    EventHandler::operator=(std::move(other));
    return *this;
}

/*----------------------------------------------------------------------------*/

ViewTransform& ViewTransform::operator=(const ViewTransform& other) {
    m_rotate_transform    = other.m_rotate_transform;
    m_translate_transform = other.m_translate_transform;
    TransformBase::operator=(other);
    EventHandler::operator=(other);
    return *this;
}

/*----------------------------------------------------------------------------*/

void ViewTransform::set_rotate_transform(RotateTransform& rotate_transform) {
    m_rotate_transform->detach_event_handler(*this);
    m_rotate_transform = &rotate_transform;
    m_rotate_transform->attach_event_handler(*this);
}

/*----------------------------------------------------------------------------*/

void ViewTransform::set_translate_transform(
    TranslateTransform& translate_transform) {
    m_translate_transform->detach_event_handler(*this);
    m_translate_transform = &translate_transform;
    m_translate_transform->attach_event_handler(*this);
}

/*----------------------------------------------------------------------------*/

void ViewTransform::handle(TransformChangeEvent& event) noexcept {
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

/**
 * ViewTransform depends on other transforms but doesn't call their reset
 * methods - just reset them explicitly
 */
void ViewTransform::on_reset() {}

/*----------------------------------------------------------------------------*/

void ViewTransform::on_update() {
    m_transform_matrix = glm::lookAt(m_translate_transform->get_translation(),
                                     m_translate_transform->get_translation() +
                                         this->get_front_vector(),
                                     this->get_up_vector());
}

/*----------------------------------------------------------------------------*/
