#include <core/Entity.hpp>
#include <core/ResourceManager.hpp>
#include <entity_components/MeshComponent.hpp>
#include <glad/glad.h>
#include <graphics/core/Program.hpp>
#include <graphics/core/Shader.hpp>
#include <io/Window.hpp>

/*----------------------------------------------------------------------------*/

// static const std::string s_base_resource_path = RESOURCE_PATH;

/*----------------------------------------------------------------------------*/

// static const std::array<float, 12> s_plane_vertices = {
//     -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f,
//     0.0f};
// static const std::array<float, 12> s_plane_normals = {
//     0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
// static const std::array<float, 8> s_plane_texture_coordinates = {
//     0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
// static const std::array<unsigned int, 12> s_plane_indices = {0, 1, 2, 0, 2,
// 3};
//
// void create_geometry(unsigned int& VAO, unsigned int& IBO,
//                      std::array<unsigned int, 3>& buffers) {
//     glGenVertexArrays(1, &VAO);
//     glBindVertexArray(VAO);
//
//     glGenBuffers(3, buffers.data());
//
//     // positions
//     glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(float) * s_plane_vertices.size(),
//                  s_plane_vertices.data(), GL_STATIC_DRAW);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,
//     nullptr);
//
//     // normals
//     glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(float) * s_plane_normals.size(),
//                  s_plane_normals.data(), GL_STATIC_DRAW);
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,
//     nullptr);
//
//     // texture coordinates
//     glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
//     glBufferData(GL_ARRAY_BUFFER,
//                  sizeof(float) * s_plane_texture_coordinates.size(),
//                  s_plane_texture_coordinates.data(), GL_STATIC_DRAW);
//     glEnableVertexAttribArray(2);
//     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2,
//     nullptr);
//
//     glGenBuffers(1, &IBO);
//
//     // indices
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//                  sizeof(unsigned int) * s_plane_indices.size(),
//                  s_plane_indices.data(), GL_STATIC_DRAW);
//
//     // unbind all
//     glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
//     glBindVertexArray(GL_NONE);
// }
//
// void cleanup_geometry(unsigned int& VAO, unsigned int IBO,
//                       std::array<unsigned int, 3>& buffers) {
//     glDeleteBuffers(3, buffers.data());
//     glDeleteBuffers(1, &IBO);
//     glDeleteVertexArrays(1, &VAO);
// }
//
// /*----------------------------------------------------------------------------*/
//
// class WindowCloseHandler : public vq::core::EventHandler {
//   public:
//     WindowCloseHandler()           = default;
//     ~WindowCloseHandler() override = default;
//
//     void handle(vq::core::Event& event) override {
//         if (auto p_casted = dynamic_cast<vq::io::KeyEvent*>(&event)) {
//             if (p_casted->key == 256) { // GLFW_KEY_ESCAPE
//                 p_casted->window.close();
//                 p_casted->set_handled();
//             }
//         }
//     }
// };
//
// /*----------------------------------------------------------------------------*/
//
int main(void) {
    //     vq::io::Window window(vq::io::Window::WindowSpecification("VQ
    //     Example")); WindowCloseHandler close_handler;
    //
    //     window.make_current();
    //     window.attach_event_handler(close_handler);
    //
    //     vq::core::ResourceManager& resource_manager =
    //         vq::core::ResourceManager::get_instance();
    //
    //     resource_manager.load_resource<vq::graphics::core::Shader>(
    //         "vertex_shader", s_base_resource_path +
    //         "shaders/passthrough.vert.glsl",
    //         vq::graphics::core::Shader::ShaderType::VERTEX);
    //     resource_manager.load_resource<vq::graphics::core::Shader>(
    //         "fragment_shader",
    //         s_base_resource_path + "shaders/passthrough.frag.glsl",
    //         vq::graphics::core::Shader::ShaderType::FRAGMENT);
    //
    //     vq::core::ResourceHandle<vq::graphics::core::Shader>
    //     vertex_shader_handle
    //     =
    //         resource_manager.get_resource<vq::graphics::core::Shader>(
    //             "vertex_shader");
    //     vq::core::ResourceHandle<vq::graphics::core::Shader>
    //         fragment_shader_handle =
    //             resource_manager.get_resource<vq::graphics::core::Shader>(
    //                 "fragment_shader");
    //
    //     resource_manager.load_resource<vq::graphics::core::Program>(
    //         "program",
    //         std::vector<vq::graphics::core::Shader*>(
    //             {&(vertex_shader_handle.get()),
    //             &(fragment_shader_handle.get())}));
    //
    //     vq::core::ResourceHandle<vq::graphics::core::Program> program_handle
    //     =
    //         resource_manager.get_resource<vq::graphics::core::Program>("program");
    //
    //     unsigned int VAO = 0;
    //     unsigned int IBO = 0;
    //     std::array<unsigned int, 3> buffers;
    //     create_geometry(VAO, IBO, buffers);
    //
    //     while (!window.should_close()) {
    //         window.update();
    //
    //         program_handle.get().bind();
    //
    //         glBindVertexArray(VAO);
    //         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    //         glDrawElements(GL_TRIANGLES, s_plane_indices.size(),
    //         GL_UNSIGNED_INT,
    //                        nullptr);
    //         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
    //         glBindVertexArray(GL_NONE);
    //
    //         program_handle.get().unbind();
    //     }
    //
    //     cleanup_geometry(VAO, IBO, buffers);
}

/*----------------------------------------------------------------------------*/
