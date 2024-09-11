#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <stdexcept>
#include <string>

void framebuffer_size_callback(GLFWwindow *const window, const int width,
                               const int height) {
  glViewport(0, 0, width, height);
}

namespace glfw {
enum class Key { escape = GLFW_KEY_ESCAPE };
enum class Action { press = GLFW_PRESS, release = GLFW_RELEASE };

void poll_events() { glfwPollEvents(); }

class Window {
public:
  GLFWwindow *window;

  Window(const Window &) = default;
  Window(Window &&) = delete;
  Window &operator=(const Window &) = default;
  Window &operator=(Window &&) = delete;
  Window(const int width, const int height, std::string const &title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // NOLINTNEXTLINE - This has to happen after the window hints
    this->window =
        glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (this->window == nullptr) {
      glfwTerminate();
      throw std::runtime_error("failed to create GLFW window");
    }
    glfwMakeContextCurrent(this->window);
    // NOLINTNEXTLINE - we need to reinterpret_cast
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
      glfwTerminate();
      throw std::runtime_error("failed to initialize GLAD");
    }

    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
  };

  ~Window() { glfwTerminate(); };

  bool should_close() const { return glfwWindowShouldClose(this->window); };
  void set_should_close(const bool value) const {
    glfwSetWindowShouldClose(this->window, value);
  };

  void swap_buffers() const { glfwSwapBuffers(this->window); };

  bool get_key(const Key key, const Action action) const {
    return glfwGetKey(this->window, static_cast<int>(key)) ==
           static_cast<int>(action);
  };
};
} // namespace glfw
