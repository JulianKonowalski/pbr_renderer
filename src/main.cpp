#include "core/Window.hpp"

int main(void) {
    pbr::core::Window window({"PBR renderer", {800, 600}});
    window.make_current();

    while (!window.should_close()) {
        window.update();
    }
}