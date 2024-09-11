#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>
#include <ostream>
#include <string>

void framebuffer_size_callback(GLFWwindow *const window, const int width,
                               const int height) {
  glViewport(0, 0, width, height);
}

namespace glfw {
enum Key { escape = GLFW_KEY_ESCAPE };
enum Action { press = GLFW_PRESS };

class Window {
public:
  GLFWwindow *window;

  Window(const int width, const int height, std::string const &title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (this->window == NULL) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return;
    }
    glfwMakeContextCurrent(this->window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      glfwTerminate();
      return;
    }

    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
  };

  ~Window() { glfwTerminate(); };

  bool should_close() const { return glfwWindowShouldClose(this->window); }
  void set_should_close(const bool value) const {
    glfwSetWindowShouldClose(this->window, value);
  }

  bool get_key(const Key key, const Action action) const {
    return glfwGetKey(this->window, key) == action;
  };
};
} // namespace glfw
