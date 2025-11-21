#include "window.hpp"
#include "artemis/core/log.hpp"
#include <print>

namespace artemis {
Window::Window() {
    if (!glfwInit()) {
        LOG_CRIT("glfwInit()");
    }
}

} // namespace artemis
