#pragma once

#include <string>

namespace vq::core {

/*----------------------------------------------------------------------------*/

class Resource {
  public:
    explicit Resource(const std::string& id) : m_id(id) { this->load(); }
    ~Resource() { this->unload(); };

    inline const std::string& get_id() const { return m_id; }

    virtual void load()   = 0;
    virtual void unload() = 0;

  private:
    std::string m_id;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::core
