#pragma once

#include "Component.hpp"

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace vq::core {

/*----------------------------------------------------------------------------*/

class Entity {
  public:
    explicit Entity(const std::string& id);
    ~Entity(void) = default;

    template <typename T, typename... Args>
    void add_component(Args&&... args);

    template <typename T>
    T* get_component(void);

    template <typename T>
    T* remove_component(void);

    inline const std::string& get_id(void) const { return m_id; }

    void render(void);
    void update(double delta_time);

  private:
    const std::string m_id;
    std::unordered_map<size_t, Component*> m_components;
};

/*----------------------------------------------------------------------------*/

template <typename T, typename... Args>
void Entity::add_component(Args&&... args) {
    static_assert(std::is_base_of_v<Component, T>,
                  "T must derive from vq::core::Component class");
    auto component_id = m_components.find(Component::get_type_id<T>());
    if (component_id != m_components.end()) {
        throw std::runtime_error("Tried to insert a duplicate component to "
                                 "an entity " +
                                 m_id + " during a call to " + __FUNCTION__);
    }
    m_components[Component::get_type_id<T>()] = new T(args...);
}

template <typename T>
T* Entity::get_component(void) {
    static_assert(std::is_base_of_v<Component, T>,
                  "T must derive from vq::core::Component class");
    auto component_it = m_components.find(Component::get_type_id<T>());
    return component_it != m_components.end()
               ? static_cast<T*>(component_it->second)
               : nullptr;
}

template <typename T>
T* Entity::remove_component(void) {
    static_assert(std::is_base_of_v<Component, T>,
                  "T must derive from vq::core::Component class");
    auto component_it = m_components.find(Component::get_type_id<T>());
    T* component      = nullptr;
    if (component_it != m_components.end()) {
        component = static_cast<T*>(component_it->second);
        m_components.erase(component_it);
    }
    return component;
}

/*----------------------------------------------------------------------------*/

} // namespace vq::core
