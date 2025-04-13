#include "GLBuffers.hpp"
#include "../utils/assertMacros.hpp"

#include <algorithm>

using namespace ecco::OpenGL;

BaseGLBuffer::BaseGLBuffer() :
    m_isGenerated(false),
    m_isBound(false) {}


VAO::VAO() :
    BaseGLBuffer() {

}
VAO::~VAO() {

}
bool VAO::GenerateBuffer() {
    ecco_assert(m_isGenerated, "GLBuffers::VAO::Generate buffer, buffer currently exists can not regenerate without deleting");
    //gen vao
    //check for errors -> make a util file for glGetError
    //also look at the APIENTRY callback usage
    m_isGenerated = true;
    return true;
}
bool VAO::DeleteBuffer() {
    if (!m_isGenerated) //can't delete non gen buffer dont tryx
        return false;

    ecco_assert(m_attachments.size() > 0, "GLBuffers::VAO::DeleteBuffer, can not delete VAO with attached attachments, remove attachments");
    if (m_isBound)
        UnbindBuffer();

    //delete gl buffer check for errors
    m_vaoHandle = 0;
    m_isGenerated = false;
    return true;
}
bool VAO::BindBuffer() {
    ecco_assert(!m_isGenerated, "GLBuffers::VAO::BindBuffer, buffer is not generated");
    //bind buffer
    m_isBound = true;
    return true;
}
bool VAO::UnbindBuffer() {
    ecco_assert(!m_isGenerated, "GLBuffers::VAO::UnindBuffer, buffer is not generated");
    //unbind buffer
    m_isBound = false;
    return true;
}

bool VAO::SetAttachment(VBOSpecifier specifier,
                        std::unique_ptr<BaseGLBuffer> attachment) {
    m_attachments.at(specifier).emplace_back(std::move(attachment));
    return true;
}
bool VAO::RemoveAttachment(VBOSpecifier specifier,
                           const BaseGLBuffer *attachment) {
        if (m_attachments.count(specifier)) {

        auto& vec = m_attachments.at(specifier);
        auto it = std::find_if(vec.begin(), vec.end(),
                               [attachment](const std::unique_ptr<BaseGLBuffer>& ptr) {
                                   return ptr.get() == attachment;
                               });

        if (it != vec.end()) {
            vec.erase(it);
            return true;
        }
    }
    return false;
}

std::unique_ptr<BaseGLBuffer>
VAO::MoveAttachment(VBOSpecifier specifier, const BaseGLBuffer* attachment)
{
    if (m_attachments.count(specifier)) {

        auto& vec = m_attachments.at(specifier);
        auto it = std::find_if(vec.begin(), vec.end(),
                               [attachment](const std::unique_ptr<BaseGLBuffer>& ptr) {
                                   return ptr.get() == attachment;
                               });

        if (it != vec.end()) {
            //move unique ptr out of vector
            auto result = std::move(*it);
            //erase from vector
            vec.erase(it);
            return result;
        }
    }
    return nullptr;
}

/* THis also needs
 * 1) Set/Reset data, set gl_static, dynamic etc..
 * ugh fuck this should be using VBO_t not specifier
 * but data would be in format std::vector<VBO_t::type> or something
 */

//this file is a clusterfuck of things that need to be done
template<VBOSpecifier S>
VAOSubBuffer<S>::VAOSubBuffer(size_t numElements)
    : BaseGLBuffer(),
      m_numElements(numElements) {

}
template<VBOSpecifier S>
VAOSubBuffer<S>::~VAOSubBuffer() {}

template<VBOSpecifier S>
bool VAOSubBuffer<S>::GenerateBuffer() {}

template<VBOSpecifier S>
bool VAOSubBuffer<S>::DeleteBuffer() {}

template<VBOSpecifier S>
bool VAOSubBuffer<S>::BindBuffer() {}

template<VBOSpecifier S>
bool VAOSubBuffer<S>::UnbindBuffer() {}
