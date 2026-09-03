#pragma once

#include "core/Event.hpp"

namespace vq::io {

/*----------------------------------------------------------------------------*/

class Window;

/*----------------------------------------------------------------------------*/

struct KeyEvent : public vq::core::EventBase {
    explicit KeyEvent(Window& window, int key, int scancode, int action)
        : window(window), key(key), scancode(scancode), action(action) {}
    Window& window;
    const int key;
    const int scancode;
    const int action;
};

struct MouseScrollEvent : public vq::core::EventBase {
    explicit MouseScrollEvent(Window& window, double scroll_x_offset,
                              double scroll_y_offset)
        : window(window), scroll_x_offset(scroll_x_offset),
          scroll_y_offset(scroll_y_offset) {}
    Window& window;
    const double scroll_x_offset;
    const double scroll_y_offset;
};

struct MouseButtonEvent : public vq::core::EventBase {
    explicit MouseButtonEvent(Window& window, int button, int action, int mods)
        : window(window), button(button), action(action), mods(mods) {}
    Window& window;
    const int button;
    const int action;
    const int mods;
};

struct MouseMoveEvent : public vq::core::EventBase {
    explicit MouseMoveEvent(Window& window, const double x_position_delta,
                            const double y_position_delta)
        : window(window), x_position_delta(x_position_delta),
          y_position_delta(y_position_delta) {}
    Window& window;
    const double x_position_delta;
    const double y_position_delta;
};

struct MouseDragEvent : public vq::core::EventBase {
    explicit MouseDragEvent(Window& window) : window(window) {}
    Window& window;
};

struct WindowResizeEvent : public vq::core::EventBase {
    explicit WindowResizeEvent(Window& window, int width, int height)
        : window(window), width(width), height(height) {}
    Window& window;
    const int width;
    const int height;
};

/*----------------------------------------------------------------------------*/

class Window final
    : public vq::core::EventEmitter<KeyEvent, MouseScrollEvent,
                                    MouseButtonEvent, MouseMoveEvent,
                                    MouseDragEvent, WindowResizeEvent> {
  public:
    struct WindowSpecification {
        WindowSpecification()
            : title("OpenGL Window"), width(800), height(600) {}

        WindowSpecification(const char* const title, int width = 800,
                            int height = 600)
            : title(title), width(width), height(height) {}

        const char* const title;
        int width;
        int height;
    };

    struct MouseState {
        double mouse_x_position;
        double mouse_y_position;
        int key;
    };

    static void unbind_all();
    static Window* get_current();

    explicit Window(const WindowSpecification& window_specification =
                        WindowSpecification());
    Window(Window& other)  = delete;
    Window(Window&& other) = delete;
    ~Window();

    inline const WindowSpecification& get_specification() const {
        return m_specification;
    }

    inline const MouseState& get_mouse_state() const { return m_mouse_state; }

    void close();
    void update();
    void make_current();
    bool should_close();

    friend void pbr_core_window_on_key_click(Window& window, int key,
                                             int scancode, int action) {
        window.emit_event<KeyEvent>(window, key, scancode, action);
    }

    friend void pbr_core_window_on_scroll(Window& window, double x_offset,
                                          double y_offset) {
        window.emit_event<MouseScrollEvent>(window, x_offset, y_offset);
    }

    friend void pbr_core_window_on_mouse_button(Window& window, int button,
                                                int action, int mods) {
        window.emit_event<MouseButtonEvent>(window, button, action, mods);
    }

    friend void pbr_core_window_on_cursor_pos_change(Window& window,
                                                     double mouse_x_position,
                                                     double mouse_y_position) {
        const double x_position_delta =
            window.m_mouse_state.mouse_x_position - mouse_x_position;
        const double y_position_delta =
            window.m_mouse_state.mouse_y_position - mouse_y_position;
        window.m_mouse_state.mouse_x_position = mouse_x_position;
        window.m_mouse_state.mouse_y_position = mouse_y_position;
        window.emit_event<MouseMoveEvent>(window, x_position_delta,
                                          y_position_delta);
    }

    friend void pbr_core_window_on_framebuffer_size_change(Window& window,
                                                           int width,
                                                           int height) {
        window.m_specification.width  = width;
        window.m_specification.height = height;
        window.emit_event<WindowResizeEvent>(window, width, height);
    }

  private:
    void* m_glfw_window;
    MouseState m_mouse_state;
    WindowSpecification m_specification;
};

/*----------------------------------------------------------------------------*/

} // namespace vq::io
