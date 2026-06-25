#include "core/Window.hpp"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <stdexcept>

/*----------------------------------------------------------------------------*/

using namespace pbr::core;

static unsigned int s_window_count;

/*----------------------------------------------------------------------------*/

Window::Window(const WindowSpecification& window_specification)
    : m_glfw_window(nullptr), m_specification(window_specification) {
    if (!s_window_count) {
        // glfwSetErrorCallback();
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
    // glfwSetKeyCallback()
    // glfwSetScrollCallback();
    // glfwSetMouseButtonCallback();
    // glfwSetCursorPosCallback();
    // glfwSetFramebufferSizeCallback();

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