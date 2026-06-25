#pragma once

namespace pbr::core {

class Window final {
  public:
    struct WindowSpecification {
        const char* const title = "OpenGL Window";
        int size[2]             = {800, 600};
    };

    Window(const WindowSpecification& window_specification);
    ~Window(void);

    static void unbind_all(void);
    static Window* get_current(void);

    inline int get_width(void) const { return m_specification.size[0]; }
    inline int get_height(void) const { return m_specification.size[1]; }
    inline const char* const get_title(void) const {
        return m_specification.title;
    }

    void close(void);
    void update(void);
    void make_current(void);
    bool should_close(void);

  private:
    void* m_glfw_window;
    WindowSpecification m_specification;
};

} // namespace pbr::core
