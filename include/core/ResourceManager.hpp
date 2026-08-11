#pragma once

#include "core/Resource.hpp"

#include <unordered_map>
#include <utility>

namespace vq::core {

/*----------------------------------------------------------------------------*/

template <typename ResourceType>
class ResourceHandle {

    friend class ResourceManager;
    using ThisType = ResourceHandle<ResourceType>;

  public:
    ResourceHandle(ResourceType* resource, ResourceManager& resource_manager);
    ~ResourceHandle();

    ResourceHandle(const ThisType& other)      = delete;
    ThisType& operator=(const ThisType& other) = delete;

    ResourceHandle(ThisType&& other);
    ThisType& operator=(ThisType&& other);

    inline bool is_valid() const { return m_resource != nullptr; }
    ResourceType& get() const { return *m_resource; }

  private:
    ResourceType* m_resource;
    ResourceManager& m_resource_manager;
};

/*----------------------------------------------------------------------------*/

class ResourceManager {
  public:
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager&)            = default;
    ResourceManager& operator=(const ResourceManager&) = default;

    ResourceManager(ResourceManager&&)            = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

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
        size_t, std::unordered_map<std::string, std::pair<Resource*, size_t>>>
        m_resources;
};

/*----------------------------------------------------------------------------*/

template <typename ResourceType>
ResourceHandle<ResourceType>::ResourceHandle(ResourceType* resource,
                                             ResourceManager& resource_manager)
    : m_resource(resource), m_resource_manager(resource_manager) {}

template <typename ResourceType>
ResourceHandle<ResourceType>::~ResourceHandle() {
    m_resource_manager.release_handle(*this);
}

template <typename ResourceType>
ResourceHandle<ResourceType>::ResourceHandle(
    ResourceHandle<ResourceType>&& other)
    : m_resource(std::move(other.m_resource)),
      m_resource_manager(other.m_resource_manager) {
    other.m_resource = nullptr;
}

template <typename ResourceType>
ResourceHandle<ResourceType>&
ResourceHandle<ResourceType>::operator=(ResourceHandle<ResourceType>&& other) {
    m_resource         = std::move(other.m_resource);
    m_resource_manager = other.m_resource_manager;
    other.m_resource   = nullptr;
    return *this;
}

/*----------------------------------------------------------------------------*/

template <typename ResourceType, typename... Args>
bool ResourceManager::load_resource(const std::string& resource_id,
                                    Args&&... args) {
    ResourceHandle<ResourceType> resource_handle =
        this->get_resource<ResourceType>(resource_id);
    if (resource_handle.is_valid()) {
        return false;
    }
    ResourceType* resource = new ResourceType(resource_id, args...);
    if (!resource->load()) {
        delete resource;
        return false;
    }
    m_resources[Resource::get_type_id<ResourceType>()].insert(
        {resource_id, std::make_pair(resource, static_cast<size_t>(0))});
    return true;
}

template <typename ResourceType>
ResourceHandle<ResourceType>
ResourceManager::get_resource(const std::string& resource_id) {
    auto& resource_map = m_resources[Resource::get_type_id<ResourceType>()];
    auto it            = resource_map.find(resource_id);

    if (it == resource_map.end()) {
        return std::move(ResourceHandle<ResourceType>(
            static_cast<ResourceType*>(nullptr), *this));
    }

    it->second.second += 1;
    return std::move(ResourceHandle<ResourceType>(
        static_cast<ResourceType*>(it->second.first), *this));
}

template <typename ResourceType>
void ResourceManager::release_handle(
    ResourceHandle<ResourceType>& resource_handle) {
    if (!resource_handle.is_valid()) {
        return;
    }

    auto& resource_map = m_resources[Resource::get_type_id<ResourceType>()];
    auto it = resource_map.find(resource_handle.m_resource->get_id());

    if (it != resource_map.end()) {
        it->second.second -= 1;
        resource_handle.m_resource = static_cast<ResourceType*>(nullptr);
    }
}

/*----------------------------------------------------------------------------*/

} // namespace vq::core
