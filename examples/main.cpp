#include <core/Entity.hpp>
#include <entity_components/MeshComponent.hpp>
#include <io/Window.hpp>

class WindowCloseHandler : public vq::core::EventHandler {
  public:
    WindowCloseHandler()           = default;
    ~WindowCloseHandler() override = default;

    void handle(vq::core::Event& event) override {
        if (auto p_casted = dynamic_cast<vq::io::KeyEvent*>(&event)) {
            if (p_casted->key == 256) { // GLFW_KEY_ESCAPE
                p_casted->window.close();
                p_casted->set_handled();
            }
        }
    }
};

int main(void) {
    vq::io::Window window(vq::io::Window::WindowSpecification("VQ Example"));
    WindowCloseHandler close_handler;

    window.make_current();
    window.attach_event_handler(close_handler);

    vq::core::Entity test_entity("test_entity");
    test_entity.add_component<vq::entity_components::MeshComponent>(
        new vq::graphics::core::Geometry(
            std::move(vq::graphics::core::Geometry::plane_geometry(
                {-1.0, -1.0}, {1.0, 1.0}, "test_geometry"))));

    while (!window.should_close()) {
        window.update();
        test_entity.render();
    }
}
