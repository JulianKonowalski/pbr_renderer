#include "core/Entity.hpp"

using namespace vq::core;

/*----------------------------------------------------------------------------*/

Entity::Entity(const std::string& id) : m_id(id) {}

/*----------------------------------------------------------------------------*/

void Entity::render() {
    for (auto& component_it : m_components) {
        component_it.second->render();
    }
}

/*----------------------------------------------------------------------------*/

void Entity::update(double delta_time) {
    for (auto& component_it : m_components) {
        component_it.second->update(delta_time);
    }
}

/*----------------------------------------------------------------------------*/
