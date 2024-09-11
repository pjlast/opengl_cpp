#include <glad/glad.h>
#include "opengl.cpp"
#include <GLFW/glfw3.h>
#include "glfw.cpp"

const char *vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragment_shader_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

void process_input(glfw::Window const &window) {
  if (window.get_key(glfw::escape, glfw::press)) {
    window.set_should_close(true);
  }
}

int main() {
  const glfw::Window window(800, 600, "Hello Traingle");

  gl::Shader vertex_shader(vertex_shader_source, gl::Vertex);
  gl::Shader fragment_shader(fragment_shader_source, gl::Fragment);

  gl::ShaderProgram shader_program;
  shader_program.attach_shader(vertex_shader);
  shader_program.attach_shader(fragment_shader);
  shader_program.link();
  shader_program.use();

  float triangle_vertices[][3] = {
      {-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f}};

  auto vbo = gl::Buffer(GL_ARRAY_BUFFER);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  vbo.bind();
  vbo.set_data(triangle_vertices);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // render loop
  while (!window.should_close()) {
    // input
    process_input(window);

    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader_program.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // check and call events and swap the buffers
    glfwSwapBuffers(window.window);
    glfwPollEvents();
  }
  return 0;
}
