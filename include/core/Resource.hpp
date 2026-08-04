#pragma once

#include <string>

namespace vq::core {

/*----------------------------------------------------------------------------*/

class Resource {
  public:
    explicit Resource(const std::string& id);
    virtual ~Resource() = default;

    Resource(Resource&& other);
    Resource& operator=(Resource&& other);

    Resource(const Resource&)            = delete;
    Resource& operator=(const Resource&) = delete;

    inline const std::string& get_id() const { return m_id; }
    inline bool is_loaded() const { return m_is_loaded; }

    bool load();
    void unload();

  protected:
    virtual bool do_load()   = 0;
    virtual void do_unload() = 0;

  private:
    std::string m_id;
    bool m_is_loaded;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::core
