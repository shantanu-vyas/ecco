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

template <> struct VBOType<VBOSpecifier::NormalInfo> {
    using type = glm::fvec3;
    static constexpr const char* m_name = "Normal3f : glm::fvec3";
    static constexpr int m_numDimensions = 3;
    static constexpr GLenum m_elementType = GL_FLOAT;
};
template <> struct VBOType<VBOSpecifier::TriangleInfo> {
    using type = glm::ivec3;
    static constexpr const char* m_name = "Triangle3i : glm::ivec3";
    static constexpr int m_numDimensions = 3;
    static constexpr GLenum m_elementType = GL_INT;
};
template <> struct VBOType<VBOSpecifier::TexCoord1DInfo> {
    using type = glm::fvec1;
    static constexpr const char* m_name = "TexCoord1D : glm::fvec1";
    static constexpr int m_numDimensions = 1;
    static constexpr GLenum m_elementType = GL_FLOAT;
};
template <> struct VBOType<VBOSpecifier::TexCoord2DInfo> {
    using type = glm::fvec2;
    static constexpr const char* m_name = "TexCoord2D : glm::fvec2";
    static constexpr int m_numDimensions = 2;
    static constexpr GLenum m_elementType = GL_FLOAT;
};
template <> struct VBOType<VBOSpecifier::InstancePositions>   {
    using type = glm::fvec3;
    static constexpr const char* m_name = "InstancePosition : glm::fvec3";
    static constexpr int m_numDimensions = 3;
    static constexpr GLenum m_elementType = GL_FLOAT;
};
template <> struct VBOType<VBOSpecifier::InstanceTransforms> {
    using type = glm::fmat4;
    static constexpr const char* m_name = "InstanceTransforms : glm::fmat3";
    static constexpr int m_numDimensionsX = 3; //i wonder if this will cause dynamic type issues in the future...x
    static constexpr int m_numDimensionsY = 3;
    static constexpr GLenum m_elementType = GL_FLOAT;
};
template <> struct VBOType<VBOSpecifier::Float1> {
    using type = glm::fvec1;
    static constexpr const char* m_name = "Float1 : glm::fvec1";
    static constexpr int m_numDimensions = 1;
    static constexpr GLenum m_elementType = GL_FLOAT;
};
template <> struct VBOType<VBOSpecifier::Float2> {
    using type = glm::fvec2;
    static constexpr const char* m_name = "Float2 : glm::fvec2";
    static constexpr int m_numDimensions = 2;
    static constexpr GLenum m_elementType = GL_FLOAT;
};
template <> struct VBOType<VBOSpecifier::Float3> {
    using type = glm::fvec3;
    static constexpr const char* m_name = "Float3 : glm::fvec3";
    static constexpr int m_numDimensions = 3;
    static constexpr GLenum m_elementType = GL_FLOAT;
};
template <> struct VBOType<VBOSpecifier::Int1> {
    using type = glm::ivec1;
    static constexpr const char* m_name = "Int1 : glm::ivec1";
    static constexpr int m_numDimensions = 1;
    static constexpr GLenum m_elementType = GL_INT;
};
template <> struct VBOType<VBOSpecifier::Int2> {
    using type = glm::ivec2;
    static constexpr const char* m_name = "Int2 : glm::ivec2";
    static constexpr int m_numDimensions = 2;
    static constexpr GLenum m_elementType = GL_INT;
};
template <> struct VBOType<VBOSpecifier::Int3> {
    using type = glm::ivec3;
    static constexpr const char* m_name = "Int3 : glm::ivec3";
    static constexpr int m_numDimensions = 3;
    static constexpr GLenum m_elementType = GL_INT;
};
template <> struct VBOType<VBOSpecifier::Mat3x3> {
    using type = glm::fmat3;
    static constexpr const char* m_name = "Mat3 : glm::fmat3";
    static constexpr int m_numDimensionsX = 3;
    static constexpr int m_numDimensionsY = 3;
    static constexpr GLenum m_elementType = GL_FLOAT;
};

template <> struct VBOType<VBOSpecifier::Mat4x4> {
    using type = glm::fmat4;
    static constexpr const char* m_name = "Mat3 : glm::fmat4";
    static constexpr int m_numDimensionsX = 4;
    static constexpr int m_numDimensionsY = 4;
    static constexpr GLenum m_elementType = GL_FLOAT;
};



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
class VAOSubBufferBase;

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
    bool SetAttachment(std::shared_ptr<VAOSubBuffer<S>> attachment);

    template <VBOSpecifier S>
    bool RemoveAttachment(std::shared_ptr<VAOSubBuffer<S>> attachment);

    //ex if vertex vbo 1 is attached and we try to attach 2 it will unattach 1 then attach 2 and return true if replace is true
    //These functions are specifically setting the vertexattribptr
    //0 - Vertex, 1 - Normal, 2 - Texture Coord, 3-N anything else
    template<VBOSpecifier S>
    bool Attach(std::shared_ptr<VAOSubBuffer<S>> attachment, bool replace = true);
    template<VBOSpecifier S>
    bool Detach(std::shared_ptr<VAOSubBuffer<S>> attachment);
protected:
    GLuint m_vaoHandle = 0;
private:
    std::map<VBOSpecifier, std::vector<std::shared_ptr<VAOSubBufferBase>>> m_attachments;

    size_t m_maxAllowedNumAttachments;
    struct AttachedAttachments {
        bool m_isAttached;
        int m_slotNumber;
        std::shared_ptr<std::shared_ptr<VAOSubBufferBase>> m_attachment;
    };
    std::map<int, std::vector<AttachedAttachments>> m_attachedAttachments;


};

class VAOSubBufferBase : public BaseGLBuffer {
public:
    VAOSubBufferBase() : BaseGLBuffer() {};
    virtual ~VAOSubBufferBase() = default;
    virtual VBOSpecifier GetVBOType() = 0;
};

template <VBOSpecifier S>
class VAOSubBuffer : public VAOSubBufferBase {
    friend class VAO;
public:
    using DataType = VBO_t<S>;

    VAOSubBuffer();
    virtual ~VAOSubBuffer();
    virtual VBOSpecifier GetVBOType() override;

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
