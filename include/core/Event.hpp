#pragma once

namespace vq::core {

/*----------------------------------------------------------------------------*/

struct Event {
  public:
    Event() : m_is_handled(false) {}
    virtual ~Event() = default;

    inline bool is_handled() const { return m_is_handled; }
    inline void set_handled() { m_is_handled = true; }

  private:
    bool m_is_handled;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::core
