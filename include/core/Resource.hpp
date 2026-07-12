#pragma once

#include <string>

namespace vq::core {

/*----------------------------------------------------------------------------*/

class Resource {
  public:
    explicit Resource(const std::string& id) : m_id(id) { this->load(); }
    ~Resource(void) { this->unload(); };

    inline const std::string& get_id(void) const { return m_id; }

    virtual void load(void)   = 0;
    virtual void unload(void) = 0;

  private:
    std::string m_id;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::core
