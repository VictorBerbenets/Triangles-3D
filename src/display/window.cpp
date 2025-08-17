#include "display/window.hpp"

#include <stdexcept>

namespace triangles {

WindowDisplay::WindowDisplay(unsigned W, unsigned H, std::string_view Name)
    : Width(W), Height(H), WindowName(Name) {
  if (!glfwInit()) {
    destroy();
    throw std::runtime_error("couldn't init a window");
  }

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  Window =
      glfwCreateWindow(Width, Height, WindowName.c_str(), nullptr, nullptr);
  glfwMakeContextCurrent(Window);
}

WindowDisplay::~WindowDisplay() { destroy(); }

void WindowDisplay::destroy() {
  glfwDestroyWindow(Window);
  glfwTerminate();
}

void WindowDisplay::display() {
  while (!shouldClose()) {
    glfwSwapBuffers(Window);
    glfwPollEvents();
  }
}

} // namespace triangles
