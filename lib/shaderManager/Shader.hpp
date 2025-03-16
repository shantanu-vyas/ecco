#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/gl.h>
#include <memory>
#include <string>
#include "../ecco/EccoProduct.hpp"
#include "UniformTypes.hpp"
#include "ShaderProgram.hpp"

namespace ecco
{
    namespace OpenGL
    {
        enum ShaderType {
        ST_None,
        ST_Vertex,
        ST_Fragment,
        ST_Geometry };

        enum LoadType {
        LT_FromSource,
        LT_FromBinarySource };


        class Shader : public ecco::EccoProduct
        {
            friend ShaderProgram; //Shader Program will need to add it self

            public:
                Shader() = delete;
                Shader(Shader&) = delete;
                Shader(const Shader&) = delete;
                ~Shader() {}; //needs to delete the shader object

                static std::string GetName() {return "SHADER";};

                bool GetIsLoaded() const;
                LoadType GetLoadType() const;
                bool GetIsCompiled() const;
                std::vector<std::weak_ptr<ShaderProgram>> GetAttachedShaderPrograms() const;
                bool GetCompileLog() const;
                ShaderType GetShaderType() const;

                /* THIS MAY NOT WORK THE BEST AND IS FOR EARLY DEBUGGING
                 * Especially once i start defining structs/complex types inside of shaders i will need a way
                 * for the parser to parse them and a way to add them to the uniformtypes lists
                 *
                 * OpenGL doesn't let you query a shader directly for its uniforms so this will regex a file
                 * We can query a shaderprogram for its ACTIVE_UNIFORMS but we cant tie any uniforms to a specific shader
                 * */
                void ParseShaderForUniforms();
                std::map<std::string, UniformType> GetUniforms() const;

                //didn't write the manager yet but i wanna make sure this works
                //protected:
                //move this to the cpp file eventually when i impl the other function
                Shader(const std::string& name, const std::string& filename, LoadType loadType, ShaderType shaderType) :
                    ecco::EccoProduct("Shader-" + name),
                    m_filename(filename),
                    m_loadType(loadType),
                    m_shaderType(shaderType)
                {
                    // if (m_loadType == LT_FromSource)
                    //     m_isLoaded = loadFromFile(filename);
                    // else if (m_loadType == LT_FromBinarySource)
                    //     m_isLoaded = loadFromCachedBinary(filename);
                }

                //Only ShaderManager and ShaderProgram should be able to call these
                [[nodiscard]] GLuint GetShaderID() const { return m_shaderID; };
                //These are all marked nodiscard because we need to throw or something if they come back bad
                //OpenGL lets you operate with unloaded/noncompiled shaders and it will lead to fuckyness

                [[nodiscard]] bool ReloadFromFile(std::string filename);
                [[nodiscard]] bool SaveCachedBinary(std::string filename);

                bool AddShaderProgramUsage(std::shared_ptr<ShaderProgram> program);
                bool RemoveShaderProgramUsage(std::shared_ptr<ShaderProgram> program);

            private:
                [[nodiscard]] bool loadFromFile(std::string filename);
                [[nodiscard]] bool loadFromCachedBinary(std::string filename);

                /* I need to think about who will call this. The user will modify a shader at runtime and we need to relink
                 * all ShaderPrograms that are using it so this should be called by a public reload func?
                 * in which case the protected reloadfromfile and savecachedbinary files need to be public... figure this out
                 * after writing the shaderprogram and writing the impl for this file */

                [[nodiscard]] bool relinkAttachedPrograms();

                std::string m_filename;
                LoadType m_loadType;

                GLuint m_shaderID;
                bool m_isLoaded;

                bool m_isCompiled;
                std::vector<std::weak_ptr<ShaderProgram>> m_attachedShaderProgams;
                std::string m_compileLog;
                ShaderType m_shaderType;

                std::map<std::string, UniformType> m_uniforms;

        };
    }
}

#endif
