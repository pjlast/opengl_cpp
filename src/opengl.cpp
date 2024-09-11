#include <glad/glad.h>
#include <string>
#include <stdexcept>
#include <array>

namespace gl {

class VertexArray {
public:
  unsigned int id{};
  VertexArray() { glGenVertexArrays(1, &this->id); }
  VertexArray(const VertexArray &) = default;
  VertexArray(VertexArray &&) = delete;
  VertexArray &operator=(const VertexArray &) = default;
  VertexArray &operator=(VertexArray &&) = delete;
  ~VertexArray() { glDeleteVertexArrays(1, &this->id); }
  void bind() const { glBindVertexArray(this->id); }
};

class Buffer {
public:
  unsigned int id{};
  int buffer_type;

  Buffer(const int buffer_type) : buffer_type(buffer_type) {
    glGenBuffers(1, &this->id);
  };
  Buffer(const Buffer &) = default;
  Buffer(Buffer &&) = delete;
  Buffer &operator=(const Buffer &) = default;
  Buffer &operator=(Buffer &&) = delete;

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

  Shader(const Shader &) = default;
  Shader(Shader &&) = delete;
  Shader &operator=(const Shader &) = default;
  Shader &operator=(Shader &&) = delete;
  Shader(const char *const shader_source, ShaderType shader_type)
      : id(glCreateShader(shader_type)) {

    glShaderSource(id, 1, &shader_source, nullptr);
    glCompileShader(id);

    int success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
      std::array<char, 512> info_log{};
      glGetShaderInfoLog(id, 512, nullptr, info_log.data());
      glDeleteShader(id);
      std::string error_msg = "ERROR::SHADER::COMPILATION_FAILED\n";
      error_msg.append(info_log.data());
      throw std::runtime_error(error_msg);
    }
  };

  ~Shader() { glDeleteShader(id); };
};

class ShaderProgram {
public:
  unsigned int id;

  ShaderProgram() : id(glCreateProgram()) {};
  ShaderProgram(const ShaderProgram &) = default;
  ShaderProgram(ShaderProgram &&) = delete;
  ShaderProgram &operator=(const ShaderProgram &) = default;
  ShaderProgram &operator=(ShaderProgram &&) = delete;

  void use() const { glUseProgram(id); };
  void attach_shader(const Shader &shader) const {
    glAttachShader(this->id, shader.id);
  }
  void link() const { glLinkProgram(this->id); }

  ~ShaderProgram() { glDeleteProgram(id); };
};

} // namespace gl
