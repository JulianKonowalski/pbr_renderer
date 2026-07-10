#include "core/Window.hpp"

#include <iostream>

pbr::core::EventLoop loop;

class CustomHandler : public pbr::core::EventHandler {
  public:
    CustomHandler(void) : pbr::core::EventHandler(loop) {}
    ~CustomHandler(void) = default;

    void handle(pbr::core::Event& event) override {
        if (auto* e = dynamic_cast<pbr::core::WindowResizeEvent*>(&event)) {
            std::cout << "X: " << e->window.get_specification().width << " "
                      << "Y: " << e->window.get_specification().height << "\n";
        }
    }
};

int main(void) {
    pbr::core::Window window(loop, {"PBR renderer"});

    CustomHandler handler;
    window.make_current();

    while (!window.should_close()) {
        window.update();
    }
}