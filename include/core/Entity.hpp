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
    explicit Entity(const std::string& id) : m_id(id) {}
    ~Entity(void) = default;

    inline const std::string& get_id(void) const { return m_id; }

    template <typename T, typename... Args>
    void add_component(Args&&... args) {
        static_assert(std::is_base_of_v<Component, T>,
                      "T must derive from vq::core::Component class");
        auto component_id = m_components.find(Component::get_type_id<T>());
        if (component_id != m_components.end()) {
            throw std::runtime_error("Tried to insert a duplicate component to "
                                     "an entity %s during a call to %s",
                                     m_id, __FUNCTION__);
        }
        m_components[Component::get_type_id<T>()] =
            new T(std::forward(args)...);
    }

    template <typename T>
    T* get_component(void) {
        static_assert(std::is_base_of_v<Component, T>,
                      "T must derive from vq::core::Component class");
        auto component_it = m_components.find(Component::get_type_id<T>());
        return component_it != m_components.end()
                   ? static_cast<T*>(component_it->second)
                   : nullptr;
    }

    template <typename T>
    T* remove_component(void) {
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

    void render(void) {
        for (auto& component_it : m_components) {
            component_it.second->render();
        }
    }

    void update(double delta_time) {
        for (auto& component_it : m_components) {
            component_it.second->update(delta_time);
        }
    }

  private:
    const std::string m_id;
    std::unordered_map<size_t, Component*> m_components;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::core
