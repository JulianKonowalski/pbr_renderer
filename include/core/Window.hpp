#pragma once

#include "Event.hpp"

#include <array>
#include <iostream>

namespace pbr::core {

/*----------------------------------------------------------------------------*/

class Window;

/*----------------------------------------------------------------------------*/

struct KeyEvent : public Event {
    explicit KeyEvent(Window& window, int key, int scancode, int action)
        : window(window), key(key), scancode(scancode), action(action) {}
    Window& window;
    const int key;
    const int scancode;
    const int action;
};

/*----------------------------------------------------------------------------*/

struct MouseScrollEvent : public Event {
    explicit MouseScrollEvent(Window& window, double scroll_x_offset,
                              double scroll_y_offset)
        : window(window), scroll_x_offset(scroll_x_offset),
          scroll_y_offset(scroll_y_offset) {}
    Window& window;
    const double scroll_x_offset;
    const double scroll_y_offset;
};

/*----------------------------------------------------------------------------*/

struct MouseButtonEvent : public Event {
    explicit MouseButtonEvent(Window& window, int button, int action, int mods)
        : window(window), button(button), action(action), mods(mods) {}
    Window& window;
    const int button;
    const int action;
    const int mods;
};

/*----------------------------------------------------------------------------*/

struct MouseMoveEvent : public Event {
    explicit MouseMoveEvent(Window& window) : window(window) {}
    Window& window;
};

/*----------------------------------------------------------------------------*/

struct MouseDragEvent : public Event {
    explicit MouseDragEvent(Window& window) : window(window) {}
    Window& window;
};

/*----------------------------------------------------------------------------*/

struct WindowResizeEvent : public Event {
    explicit WindowResizeEvent(Window& window) : window(window) {}
    Window& window;
};

/*----------------------------------------------------------------------------*/

class Window final : public EventDispatcher<KeyEvent, MouseScrollEvent,
                                            MouseButtonEvent, MouseMoveEvent,
                                            MouseDragEvent, WindowResizeEvent> {
  public:
    struct WindowSpecification {
        WindowSpecification(void) : title("OpenGL Window"), size({800, 600}) {}

        WindowSpecification(const char* const title, int width, int height)
            : title(title), size({width, height}) {}

        WindowSpecification(const char* const title,
                            const std::array<int, 2>& size)
            : title(title), size(size) {}

        const char* const title;
        std::array<int, 2> size;
    };

    struct MouseState {
        double mouse_x_position;
        double mouse_y_position;
        int key;
    };

    static void unbind_all(void);
    static Window* get_current(void);

    Window(const WindowSpecification& window_specification =
               WindowSpecification());
    ~Window(void);

    inline const WindowSpecification& get_specification(void) const {
        return m_specification;
    }

    inline const MouseState& get_mouse_state(void) const {
        return m_mouse_state;
    }

    void close(void);
    void update(void);
    void make_current(void);
    bool should_close(void);

    friend void pbr_core_window_on_key_click(Window& window, int key,
                                             int scancode, int action) {
        window.dispatch_event<KeyEvent>(window, key, scancode, action);
    }

    friend void pbr_core_window_on_scroll(Window& window, double x_offset,
                                          double y_offset) {
        window.dispatch_event<MouseScrollEvent>(window, x_offset, y_offset);
    }

    friend void pbr_core_window_on_mouse_button(Window& window, int button,
                                                int action, int mods) {
        window.dispatch_event<MouseButtonEvent>(window, button, action, mods);
    }

    friend void pbr_core_window_on_cursor_pos_change(Window& window,
                                                     double mouse_x_position,
                                                     double mouse_y_position) {
        window.dispatch_event<MouseMoveEvent>(window);
    }

    friend void pbr_core_window_on_framebuffer_size_change(Window& window,
                                                           int width,
                                                           int height) {
        window.dispatch_event<WindowResizeEvent>(window);
    }

  private:
    void* m_glfw_window;
    MouseState m_mouse_state;
    WindowSpecification m_specification;
};

/*----------------------------------------------------------------------------*/

} // namespace pbr::core
