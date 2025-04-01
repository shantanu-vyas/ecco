#include "ShaderProgram.hpp"
#include "../utils/assertMacros.hpp"
#include "shaderManager/Shader.hpp"
#include <memory>

using namespace ecco::OpenGL;

GLuint ShaderProgram::getShaderProgramID() {
    return m_shaderProgramID;
}

bool ShaderProgram::reloadShaderProgram() {
    return false;
}


void ShaderProgram::setVertexShaders(const std::vector<std::shared_ptr<Shader>> &vertexShaders) {

}
void ShaderProgram::addVertexShader(const std::shared_ptr<Shader> shader) {

}

void ShaderProgram::setFragmentShaders(const std::vector<std::shared_ptr<Shader>> &fragmentShaders) {

}
void ShaderProgram::addFragmentShader(const std::shared_ptr<Shader> shader) {

}

void ShaderProgram::setGeometryShaders(const std::vector<std::shared_ptr<Shader>> &fragmentShaders) {

}
void ShaderProgram::addGeometryShader(const std::shared_ptr<Shader> shader) {

}

bool ShaderProgram::linkShaders() {
    return false;
}

ShaderProgram::ShaderProgram(std::string name,
                std::variant<std::shared_ptr<Shader>, const std::vector<std::shared_ptr<Shader>>> vertexShaders,
                std::variant<std::shared_ptr<Shader>, const std::vector<std::shared_ptr<Shader>>> fragShaders,
                std::variant<std::shared_ptr<Shader>, const std::vector<std::shared_ptr<Shader>>> geometryShaders) :
    ecco::EccoProduct(name) {

    if (std::holds_alternative<std::shared_ptr<Shader>>(vertexShaders))
        m_vertexShaders.emplace_back(std::get<std::shared_ptr<Shader>>(vertexShaders));
    else
        m_vertexShaders = std::get<const std::vector<std::shared_ptr<Shader>>>(vertexShaders);

    if (std::holds_alternative<std::shared_ptr<Shader>>(fragShaders))
        m_fragShaders.emplace_back(std::get<std::shared_ptr<Shader>>(fragShaders));
    else
        m_fragShaders = std::get<const std::vector<std::shared_ptr<Shader>>>(fragShaders);

    if (std::holds_alternative<std::shared_ptr<Shader>>(geometryShaders))
        m_geometryShaders.emplace_back(std::get<std::shared_ptr<Shader>>(geometryShaders));
    else
        m_geometryShaders = std::get<const std::vector<std::shared_ptr<Shader>>>(geometryShaders);

    //run this lambda against all 3 vectoru
    auto checker = [this](std::vector<std::shared_ptr<Shader>> shaders,  ShaderType type) -> void {
        for (const auto& shader : shaders)
            ecco_assert(shader->GetShaderType() != type, "Non valid shader type for (use magic enum to convert to string)" );
    };
    checker(m_vertexShaders, ST_Vertex);
    checker(m_fragShaders, ST_Fragment);
    checker(m_geometryShaders, ST_Geometry);
}

ShaderProgram::ShaderProgram(std::string name,
                std::variant<std::shared_ptr<Shader>, const std::vector<std::shared_ptr<Shader>>> vertexShaders,
                std::variant<std::shared_ptr<Shader>, const std::vector<std::shared_ptr<Shader>>> fragShaders) :
    ShaderProgram(name,
                  vertexShaders,
                  fragShaders,
                  std::vector<std::shared_ptr<Shader>>()) {

}
