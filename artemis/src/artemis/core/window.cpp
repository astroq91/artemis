#include "window.hpp"
#include "artemis/core/application.hpp"
#include "artemis/core/log.hpp"
#include <print>

namespace artemis {
Window::Window() {
    auto logger = Log::logger();
    if (!glfwInit()) {
        logger->critical("glfwInit()");
    }
}

} // namespace artemis
