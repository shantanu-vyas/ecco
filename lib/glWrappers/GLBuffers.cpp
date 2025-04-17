
#include "GLBuffers.hpp"
#include "GLUtils.hpp"

#include "../utils/assertMacros.hpp"
#include "glWrappers/Attachment.hpp"

#include <algorithm>
#include <memory>

using namespace ecco::OpenGL;

BaseGLBuffer::BaseGLBuffer() :
    m_isGenerated(false),
    m_isBound(false) {}


VAO::VAO() :
    BaseGLBuffer() {
    size_t maxNumAttachements = ecco::OpenGL::GetMaxAllowedVAOAttachments();
    AttachedAttachments empty{false, nullptr};
    for (size_t i = 0; i < maxNumAttachements; i++)
        m_attachedAttachments[i] = empty;

    //this has to be done for all types to initially put in map
    m_attachments[VBOSpecifier::VertexInfo];

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
bool VAO::Attach(std::shared_ptr<VAOSubBuffer<S>> attachment, bool replace) {
    auto& vec = m_attachments.at(S);
    ecco_assert(std::find(vec.begin(), vec.end(), attachment) == vec.end(), "VAO::Attach - Can not attach non added attachment");

    if (S == VBOSpecifier::VertexInfo) {
        // if (!replace)
            //if currently bound return false
        //bind vertex attrib array at 0

        auto cur = m_attachedAttachments.at(0);
        cur.m_isAttached = true;
        cur.m_attachment = attachment;
        cur.m_attachment->SetAttachmentSlot(0);

    }
    else if (S == VBOSpecifier::NormalInfo) {
        // if (!replace)
            //same thing
        auto cur = m_attachedAttachments.at(1);
        cur.m_isAttached = true;
        cur.m_attachment = attachment;
        cur.m_attachment->SetAttachmentSlot(1);
    }
    else if (S == VBOSpecifier::TriangleInfo) {
        // if (!replace)
            //same thing but no vertexattribarrayptr

        auto cur = m_attachedAttachments.at(2);
        cur.m_isAttached = true;
        cur.m_attachment = attachment;
        cur.m_attachment->SetAttachmentSlot(2);
    }
    else if (S == VBOSpecifier::TexCoord1DInfo || S == VBOSpecifier::TexCoord2DInfo) {

        // if (!replace)
            //same thing
        auto cur = m_attachedAttachments.at(3);
        cur.m_isAttached = true;
        cur.m_attachment = attachment;
        cur.m_attachment->SetAttachmentSlot(3);
    }
    else {
        auto findOpenSlot = [this]() -> int {
            for (const auto& it : m_attachedAttachments)
                if (it.second.m_attachment == nullptr && it.first > 3) //0 1 2 3 are dedicated for mesh
                    return it.first;
            return -1;
        };
        auto openSlot = findOpenSlot();
        m_attachedAttachments.at(openSlot) = attachment;
        auto cur = m_attachedAttachments.at(openSlot);
        cur.m_isAttached = true;
        cur.m_attachment = attachment;
        cur.m_attachment->SetAttachmentSlot(openSlot);
    }
}


template<VBOSpecifier S>
bool VAO::Detach(std::shared_ptr<VAOSubBuffer<S>> attachment) {
    //detach in gl
    auto& attachedAttachment = m_attachedAttachments.at(attachment->GetSlotID);
    attachedAttachment->m_isAttached = false;
    attachment->SetSlotID(0);
    attachedAttachment->m_attachment = nullptr;
}

void VAO::PrintAllAttachments() const {
    for (const auto& i : m_attachments) {
        std::cout << "ATTACHMENT TYPE: " << (int)i.first << std::endl;
        for (const auto& a : i.second) {
            std::cout << a.get() << std::endl;
        }
    }
}

void VAO::PrintAttachedAttachments() const {

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
template bool ecco::OpenGL::VAO::SetAttachment<ecco::OpenGL::VBOSpecifier::VertexInfo>(std::shared_ptr<ecco::OpenGL::VAOSubBuffer<ecco::OpenGL::VBOSpecifier::VertexInfo>>);
template bool ecco::OpenGL::VAO::RemoveAttachment<ecco::OpenGL::VBOSpecifier::VertexInfo>(std::shared_ptr<ecco::OpenGL::VAOSubBuffer<ecco::OpenGL::VBOSpecifier::VertexInfo>>);
template bool ecco::OpenGL::VAO::Attach<(ecco::OpenGL::VBOSpecifier)0>(std::shared_ptr<ecco::OpenGL::VAOSubBuffer<(ecco::OpenGL::VBOSpecifier)0> >, bool);
template bool ecco::OpenGL::VAO::Detach<(ecco::OpenGL::VBOSpecifier)0>(std::shared_ptr<ecco::OpenGL::VAOSubBuffer<(ecco::OpenGL::VBOSpecifier)0> >);


template class ecco::OpenGL::VAOSubBuffer<VBOSpecifier::NormalInfo>;
template class ecco::OpenGL::VAOSubBuffer<VBOSpecifier::TriangleInfo>;
template class ecco::OpenGL::VAOSubBuffer<VBOSpecifier::TexCoord1DInfo>;
template class ecco::OpenGL::VAOSubBuffer<VBOSpecifier::TexCoord2DInfo>;
template class ecco::OpenGL::VAOSubBuffer<VBOSpecifier::InstancePositions>;
template class ecco::OpenGL::VAOSubBuffer<VBOSpecifier::InstanceTransforms>;
