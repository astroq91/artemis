#pragma once
#include <GLFW/glfw3.h>

namespace artemis {
class Window {
  public:
    Window();

  private:
    GLFWwindow* window_;
};
} // namespace artemis
