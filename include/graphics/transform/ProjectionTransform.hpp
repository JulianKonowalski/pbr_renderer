#pragma once

#include "graphics/transform/TransformBase.hpp"

namespace vq::graphics::transform {

/*----------------------------------------------------------------------------*/

class ProjectionBase : public TransformBase {
  public:
    ProjectionBase(const float far_clip_distance,
                   const float near_clip_distance,
                   const float viewport_aspect_ratio);
    ~ProjectionBase() = default;

    ProjectionBase(ProjectionBase&& other);
    ProjectionBase(const ProjectionBase& other);

    ProjectionBase& operator=(ProjectionBase&& other);
    ProjectionBase& operator=(const ProjectionBase& other);

    inline float get_far_clip_distance() const;
    inline float get_near_clip_distance() const;
    inline float get_viewport_aspect_ratio() const;

    void set_far_clip_distance(const float far_clip_distance);
    void set_near_clip_distance(const float near_clip_distance);
    void set_viewport_aspect_ratio(const float viewport_aspect_ratio);

  protected:
    void on_reset() override;

    float m_far_clip_distance;
    float m_near_clip_distance;
    float m_viewport_aspect_ratio;
};

/*----------------------------------------------------------------------------*/

class PerspectiveProjection final : public ProjectionBase {
  public:
    PerspectiveProjection(const float field_of_view,
                          const float far_clip_distance,
                          const float near_clip_distance,
                          const float viewport_aspect_ratio);
    ~PerspectiveProjection() = default;

    PerspectiveProjection(PerspectiveProjection&& other);
    PerspectiveProjection(const PerspectiveProjection& other);

    PerspectiveProjection& operator=(PerspectiveProjection&& other);
    PerspectiveProjection& operator=(const PerspectiveProjection& other);

    inline float get_field_of_view() const;
    void set_field_of_view(const float field_of_view);

  protected:
    void on_update() override;

  private:
    float m_field_of_view;
};

/*----------------------------------------------------------------------------*/

class OrthographicProjection final : public ProjectionBase {
  public:
    OrthographicProjection(const float scale, const float far_clip_distance,
                           const float near_clip_distance,
                           const float viewport_aspect_ratio);
    ~OrthographicProjection() = default;

    OrthographicProjection(OrthographicProjection&& other);
    OrthographicProjection(const OrthographicProjection& other);

    OrthographicProjection& operator=(OrthographicProjection&& other);
    OrthographicProjection& operator=(const OrthographicProjection& other);

    inline float get_scale() const;
    void set_scale(const float scale);

  protected:
    void on_update() override;

  private:
    float m_scale;
};

/*----------------------------------------------------------------------------*/

inline float ProjectionBase::get_far_clip_distance() const {
    return m_far_clip_distance;
}

inline float ProjectionBase::get_near_clip_distance() const {
    return m_near_clip_distance;
}

inline float ProjectionBase::get_viewport_aspect_ratio() const {
    return m_viewport_aspect_ratio;
}

/*----------------------------------------------------------------------------*/

inline float PerspectiveProjection::get_field_of_view() const {
    return m_field_of_view;
}

/*----------------------------------------------------------------------------*/

inline float OrthographicProjection::get_scale() const { return m_scale; }

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::transform
