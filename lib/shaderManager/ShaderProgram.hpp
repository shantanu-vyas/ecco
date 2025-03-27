#ifndef SHADER_PROGRAM
#define SHADER_PROGRAM

#include "../ecco/EccoProduct.hpp"
#include "Shader.hpp"
// #include "Shader.hpp"
#include <memory>
#include <variant>

namespace ecco {
namespace OpenGL {
  class Shader;
  enum ShaderType : unsigned int; //Forward declaring enums requires defining the type

class ShaderProgram : public ecco::EccoProduct,
                      public std::enable_shared_from_this<ShaderProgram> {
  public:
  ShaderProgram() = delete;
  ShaderProgram(ShaderProgram &) = delete;
  ShaderProgram(const ShaderProgram &) = delete;
  ~ShaderProgram() = default;

  protected:
  [[nodiscard]] GLuint getShaderProgramID();
  [[nodiscard]] bool reloadShaderProgram();
  std::shared_ptr<ShaderProgram> Copy();

  void setShadersForSingleType(const std::vector<std::shared_ptr<Shader>>& shaders, ShaderType type);

  void setVertexShaders(const std::vector<std::shared_ptr<Shader>> &vertexShaders);
  void addVertexShader(const std::shared_ptr<Shader> shader);

  void setFragmentShaders(const std::vector<std::shared_ptr<Shader>>& fragmentShaders);
  void addFragmentShader(std::shared_ptr<Shader> shader);

  void setGeometryShaders(const std::vector<std::shared_ptr<Shader>> &fragmentShaders);
  void addGeometryShader(std::shared_ptr<Shader> shader);

  bool linkShaders();

  ShaderProgram(std::string name,
                std::variant<std::shared_ptr<Shader>, const std::vector<std::shared_ptr<Shader>>&> vertexShaders,
                std::variant<std::shared_ptr<Shader>, const std::vector<std::shared_ptr<Shader>>&> fragShaders,
                std::variant<std::shared_ptr<Shader>, const std::vector<std::shared_ptr<Shader>>&> geometryShaders);
  private:
    GLuint m_shaderProgramID;
    bool m_isLinked;
    std::string m_linkLog;

    //do we get anything out of templating shader on shadertype?
    //maybe typedefing would be nicer
    std::vector<std::shared_ptr<Shader>> m_vertexShaders;
    std::vector<std::shared_ptr<Shader>> m_fragShaders;
    std::vector<std::shared_ptr<Shader>> m_geometryShaders;


};
} // namespace OpenGL
} // namespace ecco

#endif
