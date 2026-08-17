#pragma once

#include "core/Resource.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace vq::core {

/*----------------------------------------------------------------------------*/

template <typename ResourceType>
class ResourceHandle {
    static_assert(std::is_base_of_v<Resource, ResourceType>,
                  "ResourceType must derive from vq::core::Resource");

    friend class ResourceManager;
    using ThisType = ResourceHandle<ResourceType>;

  public:
    ~ResourceHandle();

    ResourceHandle(const ThisType& other)      = delete;
    ThisType& operator=(const ThisType& other) = delete;

    ResourceHandle(ThisType&& other);
    ThisType& operator=(ThisType&& other);

    inline bool is_valid() const { return m_resource && true; }

    inline ResourceType& get() const {
        return *(static_cast<ResourceType*>(m_resource.get()));
    }

  private:
    ResourceHandle(const std::shared_ptr<Resource>& resource,
                   ResourceManager& resource_manager);

    std::shared_ptr<Resource> m_resource;
    ResourceManager& m_resource_manager;
};

/*----------------------------------------------------------------------------*/

class ResourceManager {
  public:
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager& other)            = default;
    ResourceManager& operator=(const ResourceManager& other) = default;

    ResourceManager(ResourceManager&& other)            = delete;
    ResourceManager& operator=(ResourceManager&& other) = delete;

    static ResourceManager& get_instance();

    template <typename ResourceType, typename... Args>
    bool load_resource(const std::string& resource_id, Args&&... args);

    template <typename ResourceType>
    ResourceHandle<ResourceType> get_resource(const std::string& resource_id);

    template <typename ResourceType>
    void release_handle(ResourceHandle<ResourceType>& resource_handle);

  private:
    ResourceManager() = default;

    std::unordered_map<
        size_t, std::unordered_map<std::string, std::shared_ptr<Resource>>>
        m_resources;
};

/*----------------------------------------------------------------------------*/

template <typename ResourceType>
ResourceHandle<ResourceType>::ResourceHandle(
    const std::shared_ptr<Resource>& resource,
    ResourceManager& resource_manager)
    : m_resource(resource), m_resource_manager(resource_manager) {
    // don't throw on nullptr !
    if (m_resource && !dynamic_cast<ResourceType*>(m_resource.get())) {
        throw std::runtime_error(
            "Failed to create a ResourceHandle object due to a type mismatch "
            "between ResourceType template argument and actual type of the "
            "resource");
    }
}

template <typename ResourceType>
ResourceHandle<ResourceType>::~ResourceHandle() {
    m_resource_manager.release_handle<ResourceType>(*this);
}

template <typename ResourceType>
ResourceHandle<ResourceType>::ResourceHandle(
    ResourceHandle<ResourceType>&& other)
    : m_resource(std::move(other.m_resource)),
      m_resource_manager(other.m_resource_manager) {}

template <typename ResourceType>
ResourceHandle<ResourceType>&
ResourceHandle<ResourceType>::operator=(ResourceHandle<ResourceType>&& other) {
    m_resource         = std::move(other.m_resource);
    m_resource_manager = other.m_resource_manager;
    return *this;
}

/*----------------------------------------------------------------------------*/

template <typename ResourceType, typename... Args>
bool ResourceManager::load_resource(const std::string& resource_id,
                                    Args&&... args) {
    auto resource_handle = this->get_resource<ResourceType>(resource_id);
    if (resource_handle.is_valid()) {
        return false;
    }

    auto resource = std::make_shared<ResourceType>(resource_id, args...);
    if (!resource.get()->load()) {
        return false;
    }

    m_resources[Resource::get_type_id<ResourceType>()].insert(
        {resource_id, resource});
    return true;
}

template <typename ResourceType>
ResourceHandle<ResourceType>
ResourceManager::get_resource(const std::string& resource_id) {
    auto& resource_map = m_resources[Resource::get_type_id<ResourceType>()];
    auto it            = resource_map.find(resource_id);

    return it != resource_map.end()
               ? std::move(ResourceHandle<ResourceType>(it->second, *this))
               : std::move(ResourceHandle<ResourceType>(
                     std::shared_ptr<Resource>(nullptr), *this));
}

template <typename ResourceType>
void ResourceManager::release_handle(
    ResourceHandle<ResourceType>& resource_handle) {
    if (!resource_handle.is_valid()) {
        return;
    }

    auto& resource_map = m_resources[Resource::get_type_id<ResourceType>()];
    auto it            = resource_map.find(resource_handle.get().get_id());
    if (it == resource_map.end()) {
        return;
    }

    resource_handle.m_resource.reset();
    if (it->second.use_count() == 1) {
        it->second.reset();
        resource_map.erase(it);
    }
}

/*----------------------------------------------------------------------------*/

} // namespace vq::core
