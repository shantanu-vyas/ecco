#ifndef BUFFERS
#define BUFFERS

#include <GL/glew.h>
#include <GL/gl.h>

#include <memory>
#include <glm/glm.hpp>
#include <any>
#include <vector>
#include <map>

namespace ecco {
namespace OpenGL {

enum class VBOSpecifier {
    VertexInfo,
    NormalInfo,
    TriangleInfo,
    TexCoord1DInfo,
    TexCoord2DInfo,
    InstancePositions,
    InstanceTransforms,
    PerVertexAuxInfo
};

template <VBOSpecifier> struct VBOType;
template <> struct VBOType<VBOSpecifier::VertexInfo>          { using type = glm::fvec3; };
template <> struct VBOType<VBOSpecifier::NormalInfo>          { using type = glm::fvec3; };
template <> struct VBOType<VBOSpecifier::TriangleInfo>        { using type = glm::ivec3; };
template <> struct VBOType<VBOSpecifier::TexCoord1DInfo>      { using type = glm::fvec1; };
template <> struct VBOType<VBOSpecifier::TexCoord2DInfo>      { using type = glm::fvec2; };
template <> struct VBOType<VBOSpecifier::InstancePositions>   { using type = glm::fvec3; };
template <> struct VBOType<VBOSpecifier::InstanceTransforms>  { using type = glm::fmat4; };
  //I dont really want to keep any any type like this but there could be anything really... maybe any will be its own ANYVBO class
template <> struct VBOType<VBOSpecifier::PerVertexAuxInfo>    { using type = std::any; };

template <VBOSpecifier S>
using VBO_t = typename VBOType<S>::type;

class BaseGLBuffer {
public:
    BaseGLBuffer();
    BaseGLBuffer(const BaseGLBuffer&) = delete;
    BaseGLBuffer& operator=(const BaseGLBuffer&) = delete;
    virtual ~BaseGLBuffer();

    virtual bool GenerateBuffer() = 0;
    virtual bool DeleteBuffer() = 0;
    virtual bool BindBuffer() = 0;
    virtual bool UnbindBuffer() = 0;

protected:
    bool m_isGenerated = false;
    bool m_isBound = false;
};

class VAO : public BaseGLBuffer {
public:
    VAO();
    virtual ~VAO();

    bool GenerateBuffer() override;
    bool DeleteBuffer() override;
    bool BindBuffer() override;
    bool UnbindBuffer() override;

    bool SetAttachment(VBOSpecifier specifier, std::unique_ptr<BaseGLBuffer> attachment);
    bool RemoveAttachment(VBOSpecifier specifier, const BaseGLBuffer* attachment);
    std::unique_ptr<BaseGLBuffer> MoveAttachment(VBOSpecifier specifier, const BaseGLBuffer* attachment);

private:
    GLuint m_vaoHandle = 0;
    std::map<VBOSpecifier, std::vector<std::unique_ptr<BaseGLBuffer>>> m_attachments;
};

template <VBOSpecifier S>
class VAOSubBuffer : public BaseGLBuffer {
public:
    using DataType = VBO_t<S>;

    VAOSubBuffer();
    virtual ~VAOSubBuffer();

    bool GenerateBuffer() override;
    bool DeleteBuffer() override;
    bool BindBuffer() override;
    bool UnbindBuffer() override;

private:
    std::shared_ptr<VAO> m_attachedVAO;
    GLuint m_vboHandle = 0;
};

} // namespace OpenGL
} // namespace ecco

#endif
