#pragma once
#include <GLFW/glfw3.h>

namespace artemis {
class Window {
  public:
    Window();
    ~Window();

    /**
     * Called every update tick.
     */
    void on_update();

    /**
     * Determines if the window should close.
     * @return A bool saying if it should close.
     */
    bool should_close();

    /**
     * Gets the internal GLFW window handle.
     * @return The handle
     */
    GLFWwindow* get_handle() { return window_; }

  private:
    GLFWwindow* window_;
};
} // namespace artemis
