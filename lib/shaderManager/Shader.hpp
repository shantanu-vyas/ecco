#ifndef SHADER_HPP
#define SHADER_HPP


#include <GL/glew.h>
#include <GL/gl.h>

#include "../ecco/EccoProduct.hpp"
#include "ShaderProgram.hpp"
#include "UniformTypes.hpp"

#include <memory>
#include <string>
#include <map>
#include <vector>


namespace ecco {
namespace OpenGL {


enum ShaderType : unsigned int { ST_None, ST_Vertex, ST_Fragment, ST_Geometry };

class Shader : public ecco::EccoProduct,
               public std::enable_shared_from_this<Shader> {
  // friend class ShaderProgram;

public:
  Shader() = delete;
  Shader(Shader &) = delete;
  Shader(const Shader &) = delete;
  ~Shader() = default; // needs to delete the shader object

  static std::string GetName() { return "SHADER"; };

  bool GetIsLoaded() const;
  bool GetIsCompiled() const;
  std::vector<std::weak_ptr<ecco::OpenGL::ShaderProgram>> GetAttachedShaderPrograms() const;
  std::string GetCompileLog() const;
  ShaderType GetShaderType() const;

  /* THIS MAY NOT WORK THE BEST AND IS FOR EARLY DEBUGGING
   * Especially once i start defining structs/complex types inside of shaders i
   * will need a way for the parser to parse them and a way to add them to the
   * uniformtypes lists
   *
   * OpenGL doesn't let you query a shader directly for its uniforms so this
   * will regex a file We can query a shaderprogram for its ACTIVE_UNIFORMS but
   * we cant tie any uniforms to a specific shader
   * */
  void ParseShaderForUniforms();
  std::map<std::string, UniformType> GetUniforms() const;

  // didn't write the manager yet but i wanna make sure this works
  Shader(const std::string &name, const std::string &filename,
         ShaderType shaderType);

  // Only ShaderManager and ShaderProgram should be able to call these
  [[nodiscard]] GLuint GetShaderID() const;
  [[nodiscard]] bool ReloadFromFile();
  bool AddShaderProgramUsage(std::shared_ptr<ecco::OpenGL::ShaderProgram> program);
  bool RemoveShaderProgramUsage(std::shared_ptr<ecco::OpenGL::ShaderProgram> program);

  void GetOrCreate(const std::string &name,
                   ShaderType shaderType,
                   bool forceRecompile = false);

  static void SetShaderLoadDir(const std::string &dir);

private:
  bool loadFromFile();

  bool relinkAttachedPrograms();

  std::string m_filename;

  GLuint m_shaderID = 0;
  bool m_isLoaded = false;
  bool m_isCompiled = false;
  std::vector<std::weak_ptr<ecco::OpenGL::ShaderProgram>> m_attachedShaderProgams;
  std::string m_compileLog;
  ShaderType m_shaderType = ST_None;
  std::string m_source;
  std::map<std::string, UniformType> m_uniforms;

  /* Key is here is going to be filename to ensure we don't have dupllicates and we dont clobber shaders by accident */
  static std::map<std::string, std::weak_ptr<Shader>> s_shaderCache;
  static std::string s_shaderLoadDir;
};

} // namespace OpenGL
} // namespace ecco

#endif
