#pragma once

#include "core/TypeId.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace vq::core {

/*----------------------------------------------------------------------------*/

class Resource {
  public:
    explicit Resource(const std::string& id);
    virtual ~Resource() = default;

    Resource(Resource&& other);
    Resource& operator=(Resource&& other);

    Resource(const Resource& other)            = delete;
    Resource& operator=(const Resource& other) = delete;

    template <typename ChildType>
    static inline size_t get_type_id();

    inline bool is_loaded() const;
    inline const std::string& get_id() const;

    bool load();
    bool reload();
    void unload();

  protected:
    virtual bool do_load() noexcept   = 0;
    virtual bool do_reload() noexcept = 0;
    virtual void do_unload() noexcept = 0;

  private:
    std::string m_id;
    bool m_is_loaded;
};

/*----------------------------------------------------------------------------*/

template <typename ResourceType>
class ResourceHandle {
    static_assert(std::is_base_of_v<Resource, ResourceType>,
                  "ResourceType must derive from vq::core::Resource");

    friend class ResourceManager;
    using ThisType = ResourceHandle<ResourceType>;

  public:
    ~ResourceHandle();

    ResourceHandle(ThisType&& other);
    ThisType& operator=(ThisType&& other);

    ResourceHandle(const ThisType& other)      = delete;
    ThisType& operator=(const ThisType& other) = delete;

    inline bool is_valid() const;
    inline ResourceType& get() const;

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

template <typename ChildType>
inline size_t Resource::get_type_id() {
    return TypeId<Resource>::get_type_id<ChildType>();
}

inline bool Resource::is_loaded() const { return m_is_loaded; }

inline const std::string& Resource::get_id() const { return m_id; }

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

template <typename ResourceType>
inline bool ResourceHandle<ResourceType>::is_valid() const {
    return m_resource && true;
}

template <typename ResourceType>
inline ResourceType& ResourceHandle<ResourceType>::get() const {
    return *(static_cast<ResourceType*>(m_resource.get()));
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
