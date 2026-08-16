#pragma once

#include "core/TypeId.hpp"

#include <string>

namespace vq::core {

/*----------------------------------------------------------------------------*/

class Resource {
  public:
    explicit Resource(const std::string& id, const std::string& asset_path);
    virtual ~Resource() = default;

    Resource(Resource&& other);
    Resource(const Resource&) = delete;

    Resource& operator=(Resource&& other);
    Resource& operator=(const Resource&) = delete;

    template <typename T>
    static size_t get_type_id();

    inline const std::string& get_id() const { return m_id; }
    inline const std::string& get_asset_path() const { return m_asset_path; }
    inline bool is_loaded() const { return m_is_loaded; }

    bool load();
    bool reload();
    void unload();

  protected:
    virtual bool do_load() noexcept   = 0;
    virtual bool do_reload() noexcept = 0;
    virtual void do_unload() noexcept = 0;

  private:
    std::string m_asset_path;
    std::string m_id;
    bool m_is_loaded;
};

/*----------------------------------------------------------------------------*/

template <typename T>
size_t Resource::get_type_id() {
    static_assert(std::is_base_of_v<Resource, T>,
                  "T must derive from vq::core::Resource class");
    return TypeId<Resource>::get_type_id<T>();
}

/*----------------------------------------------------------------------------*/

} // namespace vq::core
