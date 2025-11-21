#include "window.hpp"
#include "GLFW/glfw3.h"
#include "artemis/core/log.hpp"

namespace artemis {
void error_callback(int error, const char* desc) {
    Log::get()->error("GLFW error. (id: {}): {}\n", error, desc);
}
Window::Window() {
    auto logger = Log::get();
    logger->debug("Initializing window.");
    if (!glfwInit()) {
        logger->critical("glfwInit()");
    }
    glfwSetErrorCallback(error_callback);

    window_ = glfwCreateWindow(640, 480, "Title", NULL, NULL);
    if (!window_) {
        logger->critical("glfwCreateWindow()");
    }
}

Window::~Window() {
    Log::get()->debug("Destroying window.");
    glfwDestroyWindow(window_);
    glfwTerminate();
}

bool Window::should_close() { return glfwWindowShouldClose(window_); }

} // namespace artemis
