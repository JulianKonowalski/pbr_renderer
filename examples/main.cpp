#include <core/Event.hpp>
#include <core/ResourceManager.hpp>
#include <glad/glad.h>
#include <graphics/core/Shader.hpp>
#include <io/Window.hpp>

/*----------------------------------------------------------------------------*/

static const std::string s_base_resource_path = RESOURCE_PATH;

/*----------------------------------------------------------------------------*/

class WindowCloseHandler : public vq::core::EventHandler<vq::io::KeyEvent> {
  public:
    void handle(vq::io::KeyEvent& event) noexcept override {
        if (event.key == 256) { // GLFW KEY ESC
            event.window.close();
            event.set_handled();
        }
    }
};

class ShaderReloadHandler : public vq::core::EventHandler<vq::io::KeyEvent> {
  public:
    void handle(vq::io::KeyEvent& event) noexcept override {
        if (event.key == 82) { // GLFW KEY R
            auto& resource_manager = vq::core::ResourceManager::get_instance();
            auto shader_handle =
                resource_manager.get_resource<vq::graphics::core::Shader>(
                    "test_shader");
            if (shader_handle.is_valid()) {
                shader_handle.get().reload();
                event.set_handled();
            }
        }
    }
};

/*----------------------------------------------------------------------------*/

int main(void) {
    vq::io::Window window(vq::io::Window::WindowSpecification("VQ Example"));
    WindowCloseHandler close_handler;
    ShaderReloadHandler shader_reload_handler;

    window.make_current();
    window.attach_event_handler(close_handler);
    window.attach_event_handler(shader_reload_handler);

    auto& resource_manager = vq::core::ResourceManager::get_instance();
    resource_manager.load_resource<vq::graphics::core::Shader>(
        "test_shader",
        std::vector<vq::graphics::core::Shader::ShaderSourceFile>({
            {s_base_resource_path + "shaders/passthrough.vert.glsl",
             vq::graphics::core::Shader::ShaderSourceType::VERTEX},
            {s_base_resource_path + "shaders/passthrough.frag.glsl",
             vq::graphics::core::Shader::ShaderSourceType::FRAGMENT},
        }));

    auto shader_handle =
        resource_manager.get_resource<vq::graphics::core::Shader>(
            "test_shader");

    unsigned int vao;
    glCreateVertexArrays(1, &vao);

    while (!window.should_close()) {
        window.update();
        shader_handle.get().bind();

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(GL_NONE);

        shader_handle.get().unbind();
    }
}

/*----------------------------------------------------------------------------*/
