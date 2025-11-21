#pragma once
#include <GLFW/glfw3.h>

namespace artemis {
class Window {
  public:
    Window();
    ~Window();

    bool should_close();

  private:
    GLFWwindow* window_;
};
} // namespace artemis
