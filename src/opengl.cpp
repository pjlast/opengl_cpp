#include <array>
#include <glad/glad.h>
#include <stdexcept>
#include <string>

namespace gl {

class Texture {
public:
  unsigned int id{};
  Texture() { glGenTextures(1, &this->id); }
  Texture(const Texture &) = default;
  Texture(Texture &&) = delete;
  Texture &operator=(const Texture &) = default;
  Texture &operator=(Texture &&) = delete;
  ~Texture() { glDeleteTextures(1, &this->id); }

  void bind() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->id);
  }
  void load(const unsigned char *const data, const int width,
            const int height) const {
    this->bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
};

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

  template <typename T, size_t size>
  void set_data(const std::array<T, size> &buffer_data) {
    glBufferData(this->buffer_type, size * sizeof(T), buffer_data.data(),
                 GL_STATIC_DRAW);
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
