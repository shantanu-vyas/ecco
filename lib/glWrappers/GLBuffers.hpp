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
    Float1,
    Float2,
    Float3,
    Int1,
    Int2,
    Int3,
    Mat3x3,
    Mat3x4,
    Mat4x4
};

/*
 * ... maybe we keep the vbos as shared ptrs instead of unique,  we need a reference to them if we want to enable a specific
 * vbo so we need to be able to have a way to enable them into the vertex attrib arrays
 * i still dont know the use case but add the remove and move attachment functions back in now that we have a shared ptr so we can
 * access the specific element
 * modify remove/move we don't need the specifier explicitly it is the template S.. (map.at(S))
 * move templated function definitions to the headerfile so i dont have to explicitly define them in the bottom of the cpp...
 *

 * regarding vertex attrib arrays
 * keep that struct mentioned below, reserve 0 for vert, 1 for norm, 3 for tex coord, 4-N is for ??
 * make a gl util function to see how many attribs are supported, assert if they keep binding shit over that limit
 *
 * actually add in the gl calls here
 *
 * write a print tree functino like we have for scenenodes to see what attachments exist
 *
 *
 * modify these strong template types to const expr some more information, did the first one
 * also finish adding new types for other per vertex data, getting rid of std::any
 *
 *
 *  */


template <VBOSpecifier> struct VBOType;
template <> struct VBOType<VBOSpecifier::VertexInfo> {
    using type = glm::fvec3;
    static constexpr const char* m_name = "Vertex3f : glm::fvec3";
    static constexpr int m_numDimensions = 3;
    static constexpr GLenum m_elementType = GL_FLOAT;
};

template <> struct VBOType<VBOSpecifier::NormalInfo>          { using type = glm::fvec3; };
template <> struct VBOType<VBOSpecifier::TriangleInfo>        { using type = glm::ivec3; };
template <> struct VBOType<VBOSpecifier::TexCoord1DInfo>      { using type = glm::fvec1; };
template <> struct VBOType<VBOSpecifier::TexCoord2DInfo>      { using type = glm::fvec2; };
template <> struct VBOType<VBOSpecifier::InstancePositions>   { using type = glm::fvec3; };
template <> struct VBOType<VBOSpecifier::InstanceTransforms>  { using type = glm::fmat4; };
template <> struct VBOType<VBOSpecifier::Float1>              {};
template <> struct VBOType<VBOSpecifier::Float2>              {};
template <> struct VBOType<VBOSpecifier::Float3>              {};
template <> struct VBOType<VBOSpecifier::Int1>              {};
template <> struct VBOType<VBOSpecifier::Int2>              {};
template <> struct VBOType<VBOSpecifier::Int3>              {};
template <> struct VBOType<VBOSpecifier::Mat3x3>              {};
template <> struct VBOType<VBOSpecifier::Mat3x4>              {};
template <> struct VBOType<VBOSpecifier::Mat4x4>              {};



template <VBOSpecifier S>
using VBO_t = typename VBOType<S>::type;

class BaseGLBuffer {
public:
    BaseGLBuffer();
    BaseGLBuffer(const BaseGLBuffer&) = delete;
    BaseGLBuffer& operator=(const BaseGLBuffer&) = delete;
    virtual ~BaseGLBuffer() = default;

    virtual bool GenerateBuffer() = 0;
    virtual bool DeleteBuffer() = 0;
    virtual bool BindBuffer() = 0;
    virtual bool UnbindBuffer() = 0;

protected:
    bool m_isGenerated = false;
    bool m_isBound = false;

};

//Forward declare vaosubbuffer for setattachment usage
template <VBOSpecifier S>
class VAOSubBuffer;

class VAO : public BaseGLBuffer {

    template <VBOSpecifier S>
    friend class VAOSubBuffer;

public:
    VAO();
    virtual ~VAO();

    bool GenerateBuffer() override;
    bool DeleteBuffer() override;
    bool BindBuffer() override;
    bool UnbindBuffer() override;

    template <VBOSpecifier S>
    bool SetAttachment(std::unique_ptr<VAOSubBuffer<S>>& attachment);

    //how tf would this even work lol there is a unique ptr
    // template <VBOSpecifier S>
    // bool RemoveAttachment(VBOSpecifier specifier, const BaseGLBuffer* attachment);

    //same here how are you supposed to get/move an attachment
    //we have a multi map so we would have to have an id attached to each element of the multimaps vector which would be its identifier
    //this isnt really a big use case though so im just going to comment it out. I cant think of any reason i would move a vbo
    //between vaos.
    // template <VBOSpecifier S>
    // std::unique_ptr<BaseGLBuffer> MoveAttachment(VBOSpecifier specifier, const BaseGLBuffer* attachment);

protected:
    GLuint m_vaoHandle = 0;
private:
    std::map<VBOSpecifier, std::vector<std::unique_ptr<BaseGLBuffer>>> m_attachments;

    //shit we also need to keep a map of vertexAttribArray index to buffer..x
    //so we need to wrap std::unique_ptr<BaseGLBUffer> in a struct
    /**
     * struct Attachment {
     * GLuint m_vertexPointerAttribID
     * std::unique_ptr<...>
     * note we typically only have 1 ebo can we have more?
     * if so how do we specify this, might need to write a case for this
     }
     */

};

template <VBOSpecifier S>
class VAOSubBuffer : public BaseGLBuffer {
    friend class VAO;
public:
    using DataType = VBO_t<S>;

    VAOSubBuffer();
    virtual ~VAOSubBuffer();

    bool GenerateBuffer() override;
    bool DeleteBuffer() override;
    bool BindBuffer() override;
    bool UnbindBuffer() override;
    bool SetData(const std::vector<DataType>& data, bool saveLocal = false);
    void ClearCache();

protected:
    bool SetVAO(std::shared_ptr<VAO> vao);

private:
    std::weak_ptr<VAO> m_attachedVAO;
    GLuint m_vboHandle = 0;

    bool m_localSaved;
    size_t m_numElements;
    std::vector<DataType> m_data;

};

} // namespace OpenGL
} // namespace ecco

#endif
