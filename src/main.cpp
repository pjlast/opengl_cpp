// glad needs to be imported first
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include "glfw.cpp"
#include "opengl.cpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char *const vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "    ourColor = aColor;\n"
    "    TexCoord = aTexCoord;\n"
    "}\0";

const char *const fragment_shader_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "    FragColor = texture(ourTexture, TexCoord);\n"
    "}\0";

void process_input(glfw::Window const &window) {
  if (window.get_key(glfw::Key::escape, glfw::Action::press)) {
    window.set_should_close(true);
  }
}

int main() {
  const glfw::Window window(800, 600, "Hello Traingle");

  gl::Shader vertex_shader(vertex_shader_source, gl::Vertex);
  gl::Shader fragment_shader(fragment_shader_source, gl::Fragment);

  const gl::ShaderProgram shader_program;
  shader_program.attach_shader(vertex_shader);
  shader_program.attach_shader(fragment_shader);
  shader_program.link();
  shader_program.use();

  const std::array<float, 32> triangle_vertices = {
      // clang-format off
      // positions         // colors          // texture coords
       0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
       0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
      -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // top left
      // clang-format on
  };
  const std::array<unsigned int, 6> indices = {0, 1, 3, 1, 2, 3};

  gl::VertexArray vao;
  gl::Buffer vbo(GL_ARRAY_BUFFER);
  gl::Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);

  vao.bind();

  vbo.bind();
  vbo.set_data(triangle_vertices);

  ebo.bind();
  ebo.set_data(indices);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        // NOLINTNEXTLINE - have to reinterpret_cast
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        // NOLINTNEXTLINE - have to reinterpret_cast
                        reinterpret_cast<void *>(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  int width = 0, height = 0, nrChannels = 0;
  unsigned char *data =
      stbi_load("../container.jpg", &width, &height, &nrChannels, 0);

  if (!data) {
    throw std::runtime_error("Failed to load texture");
  }

  gl::Texture texture;
  texture.load(data, width, height);
  stbi_image_free(data);

  shader_program.use();
  glUniform1i(glGetUniformLocation(shader_program.id, "ourTexture"), 0);

  // render loop
  while (!window.should_close()) {
    // input
    process_input(window);

    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    texture.bind();

    shader_program.use();
    vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // check and call events and swap the buffers
    window.swap_buffers();
    glfw::poll_events();
  }
  return 0;
}
