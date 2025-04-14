
#include "GLBuffers.hpp"
#include "GLUtils.hpp"

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


template <VBOSpecifier S>
bool VAO::SetAttachment(std::shared_ptr<VAOSubBuffer<S>> attachment) {
    auto& vec = m_attachments.at(S);
    if (std::find(vec.begin(), vec.end(), attachment) == vec.end()) {
        vec.emplace_back(attachment);
        return true;
    }
    return false;
}

template <VBOSpecifier S>
bool VAO::RemoveAttachment(std::shared_ptr<VAOSubBuffer<S>> attachment) {
    auto& vec = m_attachments.at(S);
    auto it = std::find(vec.begin(), vec.end(), attachment);
    if (it != vec.end()) {
        vec.erase(it);
        return true;
    }
    return false;
}

template<VBOSpecifier S>
VAOSubBuffer<S>::VAOSubBuffer()
    : VAOSubBufferBase() {}

template<VBOSpecifier S>
VAOSubBuffer<S>::~VAOSubBuffer() {}

template<VBOSpecifier S>
VBOSpecifier VAOSubBuffer<S>::GetVBOType() {
    return S;
}

template<VBOSpecifier S>
bool VAOSubBuffer<S>::GenerateBuffer() {
    return true;
}

template<VBOSpecifier S>
bool VAOSubBuffer<S>::DeleteBuffer() {
    return true;
}

template<VBOSpecifier S>
bool VAOSubBuffer<S>::BindBuffer() {
return true;
}

template<VBOSpecifier S>
bool VAOSubBuffer<S>::UnbindBuffer() {
    return true;
}

//may also want params for dynamic vs static update
template<VBOSpecifier S>
bool VAOSubBuffer<S>::SetData(const std::vector<DataType>& data, bool saveLocal) {

    //from a design purpose these should always be 0 actually
    //while maybe too verbose we always want functinos to bind then unbind at the end

    // GLuint currentBoundVAO = m_attachedVAO->m_vaoHandle;
    // GLuint currentBoundVBO = GetCurrentBoundVBO();
    // if (!m_isBound)
    //     currentBoundVAO = GetCurrentBoundVAO();

    auto vao = m_attachedVAO.lock();
    if (vao == nullptr)
        return false;
    vao->BindBuffer();
    this->BindBuffer();

    m_numElements = data.size();
    //glBufferData
    if (saveLocal) {
        m_localSaved = true;
        m_data = data;
    }

    this->UnbindBuffer();
    vao->UnbindBuffer();
    return true;
}

template<VBOSpecifier S>
void VAOSubBuffer<S>::ClearCache() {
    m_localSaved = false;
    m_data.clear();
}

template<VBOSpecifier S>
bool VAOSubBuffer<S>::SetVAO(std::shared_ptr<VAO> vao) {
    m_attachedVAO = vao;
    return true;
}


//I dont want VAOSubBuffer to be defined in .hpp file for now..
template class ecco::OpenGL::VAOSubBuffer<VBOSpecifier::VertexInfo>;
template class ecco::OpenGL::VAOSubBuffer<VBOSpecifier::NormalInfo>;
template class ecco::OpenGL::VAOSubBuffer<VBOSpecifier::TriangleInfo>;
template class ecco::OpenGL::VAOSubBuffer<VBOSpecifier::TexCoord1DInfo>;
template class ecco::OpenGL::VAOSubBuffer<VBOSpecifier::TexCoord2DInfo>;
template class ecco::OpenGL::VAOSubBuffer<VBOSpecifier::InstancePositions>;
template class ecco::OpenGL::VAOSubBuffer<VBOSpecifier::InstanceTransforms>;
