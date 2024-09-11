#include <glad/glad.h>
#include <iostream>

namespace gl {

enum ShaderType { FRAGMENT = GL_FRAGMENT_SHADER, VERTEX = GL_VERTEX_SHADER };

class Shader {
public:
  unsigned int id;

  Shader(const char *const shader_source, ShaderType shader_type) {
    id = glCreateShader(shader_type);
    glShaderSource(id, 1, &shader_source, NULL);
    glCompileShader(id);

    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
      glGetShaderInfoLog(id, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::COMPILATION_FAILED\n"
                << infoLog << std::endl;
    }
  };

  ~Shader() { glDeleteShader(id); };
};

class ShaderProgram {
public:
  unsigned int id;

  ShaderProgram() { id = glCreateProgram(); };

  void attach_shader(const Shader &shader) const {
    glAttachShader(id, shader.id);
  };

  void link() const { glLinkProgram(id); };

  void use() const { glUseProgram(id); };

  ~ShaderProgram() { glDeleteProgram(id); };
};

} // namespace gl
