#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include "display/window.hpp"

int main() {
  triangles::WindowDisplay app{};

  try {
    app.display();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }
#if 0
glfwSetErrorCallback([](int code, const char* msg) {
        fprintf(stderr, "[GLFW] %d: %s\n", code, msg);
    });

    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Working", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
#endif
}
