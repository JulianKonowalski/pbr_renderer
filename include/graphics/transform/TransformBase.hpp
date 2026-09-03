#pragma once

#include "core/Event.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace vq::graphics::transform {

/*----------------------------------------------------------------------------*/

class TransformBase;

/*----------------------------------------------------------------------------*/

struct TransformChangeEvent : public vq::core::EventBase {
    explicit TransformChangeEvent(TransformBase& transform)
        : transform(transform) {}
    TransformBase& transform;
};

struct TransformResetEvent : public vq::core::EventBase {
    explicit TransformResetEvent(TransformBase& transform)
        : transform(transform) {}
    TransformBase& transform;
};

struct TransformUpdateEvent : public vq::core::EventBase {
    explicit TransformUpdateEvent(TransformBase& transform)
        : transform(transform) {}
    TransformBase& transform;
};

/*----------------------------------------------------------------------------*/

class TransformBase
    : public vq::core::EventEmitter<TransformChangeEvent, TransformResetEvent,
                                    TransformUpdateEvent> {
  public:
    static constexpr glm::vec3 s_world_up     = {0.0f, 1.0f, 0.0f};
    static constexpr glm::vec3 s_world_front  = {0.0f, 0.0f, 1.0f};
    static constexpr glm::vec3 s_world_right  = {1.0f, 0.0f, 0.0f};
    static constexpr glm::vec3 s_world_center = {0.0f, 0.0f, 0.0f};

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

    void set_dirty();

    glm::mat4 m_transform_matrix;

  private:
    bool m_is_dirty;
};

/*----------------------------------------------------------------------------*/

inline bool TransformBase::is_dirty() const { return m_is_dirty; }

/*----------------------------------------------------------------------------*/

} // namespace vq::graphics::transform
