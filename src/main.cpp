#include "core/Window.hpp"

pbr::core::EventLoop loop;

class CustomHandler
    : public pbr::core::EventHandler<CustomHandler,
                                     pbr::core::WindowResizeEvent,
                                     pbr::core::MouseMoveEvent> {
  public:
    CustomHandler(void)
        : pbr::core::EventHandler<CustomHandler, pbr::core::WindowResizeEvent,
                                  pbr::core::MouseMoveEvent>(*this, loop) {}
    ~CustomHandler(void) = default;

    void handle(pbr::core::WindowResizeEvent&) {}
};

int main(void) {
    pbr::core::Window window({"PBR renderer", {800, 600}});

    CustomHandler handler;
    window.make_current();

    while (!window.should_close()) {
        window.update();
    }
}