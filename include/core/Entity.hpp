#pragma once

#include "Component.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace vq::core {

/*----------------------------------------------------------------------------*/

class Entity {
  public:
    explicit Entity(const std::string& id);
    ~Entity() = default;

    inline const std::string& get_id() const;

    template <typename ComponentType, typename... Args>
    bool add_component(Args&&... args);
    template <typename ComponentType>
    std::weak_ptr<ComponentType> get_component();
    template <typename ComponentType>
    bool remove_component();

    void render();
    void update(double delta_time);

  private:
    const std::string m_id;
    std::unordered_map<size_t, std::shared_ptr<Component>> m_components;
};

/*----------------------------------------------------------------------------*/

inline const std::string& Entity::get_id() const { return m_id; }

template <typename ComponentType, typename... Args>
bool Entity::add_component(Args&&... args) {
    static_assert(std::is_base_of_v<Component, ComponentType>,
                  "T must derive from vq::core::Component class");
    auto component_it =
        m_components.find(Component::get_type_id<ComponentType>());
    if (component_it != m_components.end()) {
        return false;
    }
    m_components[Component::get_type_id<ComponentType>()] =
        std::make_shared<ComponentType>(*this, args...);
    return true;
}

template <typename ComponentType>
std::weak_ptr<ComponentType> Entity::get_component() {
    static_assert(std::is_base_of_v<Component, ComponentType>,
                  "T must derive from vq::core::Component class");
    auto component_it =
        m_components.find(Component::get_type_id<ComponentType>());
    return component_it != m_components.end()
               ? std::dynamic_pointer_cast<ComponentType>(component_it->second)
               : nullptr;
}

template <typename ComponentType>
bool Entity::remove_component() {
    static_assert(std::is_base_of_v<Component, ComponentType>,
                  "T must derive from vq::core::Component class");
    auto component_it =
        m_components.find(Component::get_type_id<ComponentType>());
    if (component_it == m_components.end()) {
        return false;
    }
    m_components.erase(component_it);
    return true;
}

/*----------------------------------------------------------------------------*/

} // namespace vq::core
