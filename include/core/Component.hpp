#pragma once

#include "TypeId.hpp"

namespace vq::core {

/*----------------------------------------------------------------------------*/

class Entity;

/*----------------------------------------------------------------------------*/

class Component {
  public:
    Component(Entity& parent) : m_parent(parent) {}
    ~Component(void) = default;

    virtual void render(void)              = 0;
    virtual void update(double delta_time) = 0;

    inline Entity& get_parent(void) { return m_parent; }

    template <typename T>
    static size_t get_type_id(void) {
        static_assert(std::is_base_of_v<Component, T>,
                      "T must derive from vq::core::Component class");
        return TypeId<Component>::get_type_id<T>();
    }

  protected:
    Entity& m_parent;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::core
