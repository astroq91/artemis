#include "window.hpp"
#include "artemis/core/log.hpp"

namespace artemis {
Window::Window() {
    auto logger = Log::logger();
    if (!glfwInit()) {
        logger->critical("glfwInit()");
    }
}

} // namespace artemis
