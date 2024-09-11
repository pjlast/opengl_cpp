#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <stdexcept>
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
      glfwTerminate();
      throw std::runtime_error("failed to create GLFW window");
    }
    glfwMakeContextCurrent(this->window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      glfwTerminate();
      throw std::runtime_error("failed to initialize GLAD");
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
