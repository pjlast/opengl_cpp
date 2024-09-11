#include <glad/glad.h>
#include <iostream>

namespace gl {

class Buffer {
public:
  unsigned int id;
  int buffer_type;

  Buffer(const int buffer_type) {
    glGenBuffers(1, &this->id);
    this->buffer_type = buffer_type;
  };

  void bind() const { glBindBuffer(this->buffer_type, this->id); };

  template <typename T> void set_data(const T &buffer_data) {
    glBufferData(this->buffer_type, sizeof(T), buffer_data, GL_STATIC_DRAW);
  }

  ~Buffer() { glDeleteBuffers(1, &this->id); };
};

enum ShaderType { Fragment = GL_FRAGMENT_SHADER, Vertex = GL_VERTEX_SHADER };

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
