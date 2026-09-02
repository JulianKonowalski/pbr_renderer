#include <core/Event.hpp>
#include <core/Resource.hpp>
#include <glad/glad.h>
#include <graphics/core/Geometry.hpp>
#include <graphics/core/Shader.hpp>
#include <graphics/transform/ProjectionTransform.hpp>
#include <graphics/transform/RotateTransform.hpp>
#include <graphics/transform/ScaleTransform.hpp>
#include <graphics/transform/TranslateTransform.hpp>
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
    resource_manager.load_resource<vq::graphics::core::Geometry>(
        "test_geometry");
    resource_manager.load_resource<vq::graphics::core::Shader>(
        "test_shader",
        std::vector<vq::graphics::core::Shader::ShaderSourceFile>({
            {s_base_resource_path + "shaders/passthrough.vert.glsl",
             vq::graphics::core::Shader::ShaderSourceType::VERTEX},
            {s_base_resource_path + "shaders/passthrough.frag.glsl",
             vq::graphics::core::Shader::ShaderSourceType::FRAGMENT},
        }));

    auto geometry_handle =
        resource_manager.get_resource<vq::graphics::core::Geometry>(
            "test_geometry");
    auto shader_handle =
        resource_manager.get_resource<vq::graphics::core::Shader>(
            "test_shader");

    geometry_handle.get()
        .set_attribute<vq::graphics::core::Geometry::Attribute::POSITION>(
            {-0.5, -0.5, 0.0, //
             0.5, -0.5, 0.0,  //
             0.0, 0.5, 0.0});
    geometry_handle.get()
        .set_attribute<vq::graphics::core::Geometry::Attribute::NORMAL>(
            {0.0, 0.0, 1.0, //
             0.0, 0.0, 1.0, //
             0.0, 0.0, 1.0});
    geometry_handle.get()
        .set_attribute<
            vq::graphics::core::Geometry::Attribute::TEXTURE_COORDINATE>(
            {0.0, 0.0, //
             1.0, 0.0, //
             0.5, 1.0});

    geometry_handle.get().set_indices({0, 1, 2});

    unsigned int vao;
    glCreateVertexArrays(1, &vao);

    unsigned int model_matrix_uniform_location =
        shader_handle.get().get_uniform_location("u_model_matrix");

    vq::graphics::transform::RotateTransform rotate_transform;
    vq::graphics::transform::TranslateTransform translate_transform;
    vq::graphics::transform::PerspectiveProjection projection_transform(
        40.0f, 100.0f, 0.01f,
        window.get_specification().width /
            static_cast<float>(window.get_specification().height));

    translate_transform.translate({0.0f, 0.0f, 10.0f});

    while (!window.should_close()) {
        window.update();

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        rotate_transform.rotate_euler_degrees(
            glm::vec3(0.005f, 0.005f, 0.005f));

        shader_handle.get().bind();
        shader_handle.get().set_uniform_mat4(
            model_matrix_uniform_location,
            projection_transform.get_transform_matrix() *
                glm::lookAt(glm::vec3({0.0f, 0.0f, 0.0f}),
                            glm::vec3({0.0f, 0.0f, 1.0f}),
                            glm::vec3({0.0f, 1.0f, 0.0f})) *
                translate_transform.get_transform_matrix() *
                rotate_transform.get_transform_matrix());
        geometry_handle.get().bind();

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        vq::graphics::core::Geometry::unbind_all();
        vq::graphics::core::Shader::unbind_all();
    }
}

/*----------------------------------------------------------------------------*/
