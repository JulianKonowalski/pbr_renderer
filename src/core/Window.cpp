#include "core/Window.hpp"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <iostream>
#include <stdexcept>

/*----------------------------------------------------------------------------*/

using namespace pbr::core;

static unsigned int s_window_count;

/*----------------------------------------------------------------------------*/

void error_callback(int error_code, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mods);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
void mouse_button_callback(GLFWwindow* window, int button, int action,
                           int mods);
void cursor_pos_callback(GLFWwindow* window, double x_position,
                         double y_position);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/*----------------------------------------------------------------------------*/

static EventLoopBase event_loop;

Window::Window(const WindowSpecification& window_specification)
    : m_glfw_window(nullptr), m_specification(window_specification),
      EventDispatcher<EventLoopBase>(event_loop) {
    if (!s_window_count) {
        glfwSetErrorCallback(error_callback);
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
    }
    s_window_count += 1;

    m_glfw_window = glfwCreateWindow(
        window_specification.size[0], window_specification.size[1],
        window_specification.title, nullptr, nullptr);
    if (!m_glfw_window) {
        throw std::runtime_error("Failed to create a GLFW window");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_glfw_window));
    glfwSetWindowUserPointer(static_cast<GLFWwindow*>(m_glfw_window), this);
    glfwSetKeyCallback(static_cast<GLFWwindow*>(m_glfw_window), key_callback);
    glfwSetScrollCallback(static_cast<GLFWwindow*>(m_glfw_window),
                          scroll_callback);
    glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(m_glfw_window),
                               mouse_button_callback);
    glfwSetCursorPosCallback(static_cast<GLFWwindow*>(m_glfw_window),
                             cursor_pos_callback);
    glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(m_glfw_window),
                                   framebuffer_size_callback);
    glfwMakeContextCurrent(nullptr);
}

/*----------------------------------------------------------------------------*/

Window::~Window(void) {
    s_window_count -= 1;
    glfwDestroyWindow(static_cast<GLFWwindow*>(m_glfw_window));
    if (!s_window_count) {
        glfwTerminate();
    }
}

/*----------------------------------------------------------------------------*/

static void unbind_all(void) { glfwMakeContextCurrent(nullptr); }

/*----------------------------------------------------------------------------*/

static Window* get_current(void) {
    GLFWwindow* current_window = glfwGetCurrentContext();
    return current_window
               ? static_cast<Window*>(glfwGetWindowUserPointer(current_window))
               : nullptr;
}

/*----------------------------------------------------------------------------*/

void Window::close(void) {
    glfwSetWindowShouldClose(static_cast<GLFWwindow*>(m_glfw_window), 1);
}

/*----------------------------------------------------------------------------*/

void Window::update(void) {
    glfwPollEvents();
    glfwSwapBuffers(static_cast<GLFWwindow*>(m_glfw_window));
}

/*----------------------------------------------------------------------------*/

void Window::make_current(void) {
    glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_glfw_window));
}

/*----------------------------------------------------------------------------*/

bool Window::should_close(void) {
    return glfwWindowShouldClose(static_cast<GLFWwindow*>(m_glfw_window));
}

/*----------------------------------------------------------------------------*/

void error_callback(int error_code, const char* description) {
    std::cout << "GLFW Error[" << error_code << "]:\n"
              << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mods) {
    Window* glfw_window_user =
        static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!glfw_window_user) {
        return;
    }
    pbr_core_window_on_key_click(*glfw_window_user, key, scancode, action);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
    Window* glfw_window_user =
        static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!glfw_window_user) {
        return;
    }
    pbr_core_window_on_scroll(*glfw_window_user, x_offset, y_offset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action,
                           int mods) {
    Window* glfw_window_user =
        static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!glfw_window_user) {
        return;
    }
    pbr_core_window_on_mouse_button(*glfw_window_user, button, action, mods);
}

void cursor_pos_callback(GLFWwindow* window, double x_position,
                         double y_position) {
    Window* glfw_window_user =
        static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!glfw_window_user) {
        return;
    }
    pbr_core_window_on_cursor_pos_change(*glfw_window_user, x_position,
                                         y_position);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    Window* glfw_window_user =
        static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!glfw_window_user) {
        return;
    }
    pbr_core_window_on_framebuffer_size_change(*glfw_window_user, width,
                                               height);
}

/*----------------------------------------------------------------------------*/
