#include "Shader.hpp"
#include "../utils/assertMacros.hpp"
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <iostream>

using namespace ecco::OpenGL;

// Static cache and directory paths
std::map<std::string, std::weak_ptr<Shader>> Shader::s_shaderCache;
std::string Shader::s_shaderLoadDir;

Shader::Shader(const std::string &name,
               const std::string &filename,
               ShaderType shaderType)
    : ecco::EccoProduct("Shader-" + name),
      m_filename(filename),
      m_shaderType(shaderType) {

    loadFromFile();

}

bool Shader::GetIsLoaded() const {
    return m_isLoaded;
}

bool Shader::GetIsCompiled() const {
    return m_isCompiled;
}

std::vector<std::weak_ptr<ShaderProgram>> Shader::GetAttachedShaderPrograms() const {
    return m_attachedShaderProgams;
}

std::string Shader::GetCompileLog() const {
    return m_compileLog;
}

ShaderType Shader::GetShaderType() const {
    return m_shaderType;
}

void Shader::ParseShaderForUniforms() {
    //not impls
}

std::map<std::string, UniformType> Shader::GetUniforms() const {
    return m_uniforms;
}

GLuint Shader::GetShaderID() const {
    return m_shaderID;
}

bool Shader::ReloadFromFile() {

    return loadFromFile();
}

bool Shader::AddShaderProgramUsage(std::shared_ptr<ShaderProgram> program) {
    std::weak_ptr<ShaderProgram> programWp = program;
    for (auto &p : m_attachedShaderProgams) {
        if (p.lock() == program) {
            return false;
        }
    }
    m_attachedShaderProgams.emplace_back(programWp);
    return true;
}

bool Shader::RemoveShaderProgramUsage(std::shared_ptr<ShaderProgram> program) {
    bool removed = false;
    for (auto it = m_attachedShaderProgams.begin(); it != m_attachedShaderProgams.end(); ) {
        if (it->lock() == program) {
            it = m_attachedShaderProgams.erase(it);
            removed = true;
        } else {
            ++it;
        }
    }
    return removed;
}

bool Shader::loadFromFile() {
    m_isLoaded   = false;
    m_isCompiled = false;
    m_compileLog.clear();

    if (!m_shaderID) {
        switch (m_shaderType) {
            case ST_Vertex:
                m_shaderID = glCreateShader(GL_VERTEX_SHADER);
                break;
            case ST_Fragment:
                m_shaderID = glCreateShader(GL_FRAGMENT_SHADER);
                break;
            case ST_Geometry:
                m_shaderID = glCreateShader(GL_GEOMETRY_SHADER);
                break;
            default:
                return false;
        }
        if (!m_shaderID) {
            return false;
        }
    }

    try {
        std::string finalPath = s_shaderLoadDir + m_filename;
        std::ifstream shaderFile(finalPath, std::ios::in | std::ios::binary);
        if (!shaderFile.is_open()) {
            throw std::ifstream::failure("Failed to open shader file at: " + finalPath);
        }
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        m_source = shaderStream.str();

        const char *srcPtr = m_source.c_str();
        GLint length = static_cast<GLint>(m_source.size());
        glShaderSource(m_shaderID, 1, &srcPtr, &length);
        glCompileShader(m_shaderID);

        GLint compileStatus = 0;
        glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &compileStatus);
        m_isCompiled = (compileStatus == GL_TRUE);

        GLint logLength = 0;
        glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 1) {
            m_compileLog.resize(logLength);
            glGetShaderInfoLog(m_shaderID, logLength, nullptr, &m_compileLog[0]);
        }
        m_isLoaded = m_isCompiled;
        return m_isCompiled;
    }
    catch (const std::ifstream::failure &e) {
        std::cerr << "File IO Error: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unexpected error while loading the shader file." << std::endl;
    }

    return false;
}


bool Shader::relinkAttachedPrograms() {
    return false;
}

/* I think this is what we want? i cant think straight rn
 * i make shader("shader1.vert");
 * ... do stuff
 * i make shader("shader1.vert") again
 * - Instead of making a new shader and compiling/linking it it should just return a shared_ptr to the existing one
 * - The exception here is if they want to force it to reload it
 *
 *
 * okay update this is not what we want, this needs to be handled outside of Shader unless we want to make a static get function.
 * I think what we will do is have ShaderManager manage both Shaders and ShaderPrograms
 * For this it will need
 * 1) Keep this static vector of <shaderFileName, shader>
 * 2) If it doesn't exist make it
 * 3) If it does exist then return a shared_ptr to the existing one, if it does exist and they force recompile it call reload/relink then return the existing one
 * */
void Shader::GetOrCreate(const std::string &name,
                         ShaderType shaderType,
                         bool forceRecompile) {

    // auto it = s_shaderCache.find(m_filename);
    // if (it != s_shaderCache.end()) {
    //     auto existing = it->second.lock();
    //     if (existing) {
    //         if (forceRecompile) {
    //             bool success = existing->ReloadFromFile();
    //             if (success)
    //                 existing->relinkAttachedPrograms();
    //             else
    //             {
    //                 std::cout << "FAILED to reload" << std::endl;
    //                 std::cout << "wtf do we do now" << std::endl;
    //                 //maybe dont crash so we can modify the shader without closing the app
    //                 //but dont try to relink so it will still use the old compiling shader until the reload is called on a
    //                 //working shader
    //             }
    //         }
    //     }
    // }


    // loadFromFile();
    // relinkAttachedPrograms();
    // s_shaderCache[m_filename] = shared_from_this();

}

void Shader::SetShaderLoadDir(const std::string &dir) {
    s_shaderLoadDir = dir;
}
