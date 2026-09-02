#include "graphics/transform/ProjectionTransform.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace vq::graphics::transform;

/*----------------------------------------------------------------------------*/

ProjectionBase::ProjectionBase(const float far_clip_distance,
                               const float near_clip_distance,
                               const float viewport_aspect_ratio)
    : m_far_clip_distance(far_clip_distance),
      m_near_clip_distance(near_clip_distance),
      m_viewport_aspect_ratio(viewport_aspect_ratio) {}

/*----------------------------------------------------------------------------*/

ProjectionBase::ProjectionBase(ProjectionBase&& other)
    : TransformBase(std::move(other)),
      m_far_clip_distance(other.m_far_clip_distance),
      m_near_clip_distance(other.m_near_clip_distance),
      m_viewport_aspect_ratio(other.m_viewport_aspect_ratio) {}

/*----------------------------------------------------------------------------*/

ProjectionBase::ProjectionBase(const ProjectionBase& other)
    : TransformBase(other), m_far_clip_distance(other.m_far_clip_distance),
      m_near_clip_distance(other.m_near_clip_distance),
      m_viewport_aspect_ratio(other.m_viewport_aspect_ratio) {}

/*----------------------------------------------------------------------------*/

ProjectionBase& ProjectionBase::operator=(ProjectionBase&& other) {
    m_far_clip_distance     = other.m_far_clip_distance;
    m_near_clip_distance    = other.m_near_clip_distance;
    m_viewport_aspect_ratio = other.m_viewport_aspect_ratio;
    TransformBase::operator=(std::move(other));
    return *this;
}

/*----------------------------------------------------------------------------*/

ProjectionBase& ProjectionBase::operator=(const ProjectionBase& other) {
    m_far_clip_distance     = other.m_far_clip_distance;
    m_near_clip_distance    = other.m_near_clip_distance;
    m_viewport_aspect_ratio = other.m_viewport_aspect_ratio;
    TransformBase::operator=(other);
    return *this;
}

/*----------------------------------------------------------------------------*/

void ProjectionBase::set_far_clip_distance(const float far_clip_distance) {
    m_far_clip_distance = far_clip_distance;
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void ProjectionBase::set_near_clip_distance(const float near_clip_distance) {
    m_near_clip_distance = near_clip_distance;
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void ProjectionBase::set_viewport_aspect_ratio(
    const float viewport_aspect_ratio) {
    m_viewport_aspect_ratio = viewport_aspect_ratio;
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

/** reset shouldn't do anything on projection classes */
void ProjectionBase::on_reset() {}

/*----------------------------------------------------------------------------*/

PerspectiveProjection::PerspectiveProjection(const float field_of_view,
                                             const float far_clip_distance,
                                             const float near_clip_distance,
                                             const float viewport_aspect_ratio)
    : ProjectionBase(far_clip_distance, near_clip_distance,
                     viewport_aspect_ratio),
      m_field_of_view(field_of_view) {}

/*----------------------------------------------------------------------------*/

PerspectiveProjection::PerspectiveProjection(PerspectiveProjection&& other)
    : ProjectionBase(std::move(other)), m_field_of_view(other.m_field_of_view) {
}

/*----------------------------------------------------------------------------*/

PerspectiveProjection::PerspectiveProjection(const PerspectiveProjection& other)
    : ProjectionBase(other), m_field_of_view(other.m_field_of_view) {}

/*----------------------------------------------------------------------------*/

PerspectiveProjection&
PerspectiveProjection::operator=(PerspectiveProjection&& other) {
    m_field_of_view = other.m_field_of_view;
    ProjectionBase::operator=(std::move(other));
    return *this;
}

/*----------------------------------------------------------------------------*/

PerspectiveProjection&
PerspectiveProjection::operator=(const PerspectiveProjection& other) {
    m_field_of_view = other.m_field_of_view;
    ProjectionBase::operator=(other);
    return *this;
}

/*----------------------------------------------------------------------------*/

void PerspectiveProjection::set_field_of_view(const float field_of_view) {
    m_field_of_view = field_of_view;
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void PerspectiveProjection::on_update() {
    m_transform_matrix =
        glm::perspective(glm::radians(m_field_of_view), m_viewport_aspect_ratio,
                         m_near_clip_distance, m_far_clip_distance);
}

/*----------------------------------------------------------------------------*/

OrthographicProjection::OrthographicProjection(
    const float scale, const float far_clip_distance,
    const float near_clip_distance, const float viewport_aspect_ratio)
    : ProjectionBase(far_clip_distance, near_clip_distance,
                     viewport_aspect_ratio),
      m_scale(scale) {}

/*----------------------------------------------------------------------------*/

OrthographicProjection::OrthographicProjection(OrthographicProjection&& other)
    : ProjectionBase(std::move(other)), m_scale(other.m_scale) {}

/*----------------------------------------------------------------------------*/

OrthographicProjection::OrthographicProjection(
    const OrthographicProjection& other)
    : ProjectionBase(other), m_scale(other.m_scale) {}

/*----------------------------------------------------------------------------*/

OrthographicProjection&
OrthographicProjection::operator=(OrthographicProjection&& other) {
    m_scale = other.m_scale;
    ProjectionBase::operator=(std::move(other));
    return *this;
}

/*----------------------------------------------------------------------------*/

OrthographicProjection&
OrthographicProjection::operator=(const OrthographicProjection& other) {
    m_scale = other.m_scale;
    ProjectionBase::operator=(other);
    return *this;
}

/*----------------------------------------------------------------------------*/

void OrthographicProjection::set_scale(const float scale) {
    m_scale = scale;
    this->set_dirty();
}

/*----------------------------------------------------------------------------*/

void OrthographicProjection::on_update() {
    const float horizontal_size = m_scale * m_viewport_aspect_ratio;
    m_transform_matrix =
        glm::ortho(-horizontal_size, horizontal_size, -m_scale, m_scale,
                   m_near_clip_distance, m_far_clip_distance);
}

/*----------------------------------------------------------------------------*/
