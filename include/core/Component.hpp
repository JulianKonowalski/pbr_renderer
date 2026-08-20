#pragma once

#include "TypeId.hpp"

namespace vq::core {

/*----------------------------------------------------------------------------*/

class Entity;

/*----------------------------------------------------------------------------*/

class Component {
  public:
    explicit Component(Entity& parent) : m_parent(parent) {}
    ~Component() = default;

    template <typename ChildType>
    static inline size_t get_type_id();

    inline Entity& get_parent();

    virtual void render()                  = 0;
    virtual void update(double delta_time) = 0;

  protected:
    Entity& m_parent;
};

/*----------------------------------------------------------------------------*/

template <typename ChildType>
inline size_t Component::get_type_id() {
    return TypeId<Component>::get_type_id<ChildType>();
}

inline Entity& Component::get_parent() { return m_parent; }

/*----------------------------------------------------------------------------*/

} // namespace vq::core
