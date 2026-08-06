#pragma once

#include <string>

namespace vq::core {

/*----------------------------------------------------------------------------*/

class Resource {
  public:
    explicit Resource(const std::string& id,
                      const std::string& asset_path = "");
    virtual ~Resource() = default;

    Resource(Resource&& other);
    Resource(const Resource&) = delete;

    Resource& operator=(Resource&& other);
    Resource& operator=(const Resource&) = delete;

    inline const std::string& get_id() const { return m_id; }
    inline const std::string& get_asset_path() const { return m_asset_path; }
    inline bool is_loaded() const { return m_is_loaded; }

    bool load();
    void unload();

  protected:
    virtual bool do_load() noexcept   = 0;
    virtual void do_unload() noexcept = 0;

  private:
    std::string m_asset_path;
    std::string m_id;
    bool m_is_loaded;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::core
