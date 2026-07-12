#pragma once

namespace vq::core {

/*----------------------------------------------------------------------------*/

struct Event {
  public:
    Event(void) : m_is_handled(false) {}
    virtual ~Event(void) = default;

    inline bool is_handled(void) const { return m_is_handled; }
    inline void set_handled(void) { m_is_handled = true; }

  private:
    bool m_is_handled;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::core
