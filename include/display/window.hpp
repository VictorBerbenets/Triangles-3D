#pragma once

#define CLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace triangles {

class TrianglesContext;

class WindowDisplay {

public:
  static constexpr int DefaultWindowWidth = 800;
  static constexpr int DefaultWindowHeight = 600;
  static constexpr std::string_view DefaultWindowName = "Triangles";

  WindowDisplay(unsigned W = DefaultWindowWidth,
                unsigned H = DefaultWindowHeight,
                std::string_view Name = DefaultWindowName);

  ~WindowDisplay();

  WindowDisplay(const WindowDisplay &) = delete;
  WindowDisplay &operator=(const WindowDisplay &) = delete;

  bool shouldClose() { return glfwWindowShouldClose(Window); }

  void display();

  friend class TrianglesContext;

private:
  void destroy();

  unsigned Width;
  unsigned Height;
  std::string WindowName;
  GLFWwindow *Window;
};

} // namespace triangles
