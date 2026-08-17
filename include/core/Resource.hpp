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
    Resource& operator=(Resource&& other);

    Resource(const Resource& other)            = delete;
    Resource& operator=(const Resource& other) = delete;

    template <typename ChildType>
    static inline size_t get_type_id() {
        return TypeId<Resource>::get_type_id<ChildType>();
    }

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
    std::string m_id;
    std::string m_asset_path;
    bool m_is_loaded;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::core
