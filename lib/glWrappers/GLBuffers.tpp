#include "GLBuffers.hpp"
#include "GLUtils.hpp"

#include "../utils/assertMacros.hpp"
#include "glWrappers/Attachment.hpp"

#include <algorithm>
#include <memory>
#include <stdexcept>

using namespace ecco::OpenGL;

BaseGLBuffer::BaseGLBuffer() :
    m_isGenerated(false),
    m_isBound(false) {}


VAO::VAO() :
    BaseGLBuffer() {
    m_maxAllowedNumAttachments = std::max<size_t>(4, ecco::OpenGL::GetMaxAllowedVAOAttachments());
    AttachedAttachments empty{false, nullptr};
    for (size_t i = 0; i < m_maxAllowedNumAttachments; ++i)
        m_attachedAttachments[i] = empty;

    for (int i = static_cast<int>(VBOSpecifier::VertexInfo);
         i <= static_cast<int>(VBOSpecifier::Mat4x4); ++i)
        m_attachments.emplace(static_cast<VBOSpecifier>(i), std::vector<std::shared_ptr<VAOSubBufferBase>>{});
}
VAO::~VAO() {}
bool VAO::GenerateBuffer() {
    ecco_assert(!m_isGenerated, "GLBuffers::VAO::GenerateBuffer buffer currently exists can not regenerate without deleting");
    m_isGenerated = true;
    return true;
}
bool VAO::DeleteBuffer() {
    if (!m_isGenerated)
        return false;
    for (const auto &kv : m_attachedAttachments)
        ecco_assert(!kv.second.m_isAttached, "GLBuffers::VAO::DeleteBuffer can not delete VAO with attached attachments, remove attachments");
    if (m_isBound)
        UnbindBuffer();
    m_vaoHandle = 0;
    m_isGenerated = false;
    return true;
}
bool VAO::BindBuffer() {
    ecco_assert(m_isGenerated, "GLBuffers::VAO::BindBuffer buffer is not generated");
    m_isBound = true;
    return true;
}
bool VAO::UnbindBuffer() {
    ecco_assert(m_isGenerated, "GLBuffers::VAO::UnindBuffer buffer is not generated");
    m_isBound = false;
    return true;
}


template <VBOSpecifier S>
bool VAO::SetAttachment(std::shared_ptr<VAOSubBuffer<S>> attachment) {
    auto &vec = m_attachments.at(S);
    if (std::find(vec.begin(), vec.end(), attachment) == vec.end()) {
        vec.emplace_back(attachment);
        return true;
    }
    return false;
}

template <VBOSpecifier S>
bool VAO::RemoveAttachment(std::shared_ptr<VAOSubBuffer<S>> attachment) {
    auto &vec = m_attachments.at(S);
    auto it = std::find(vec.begin(), vec.end(), attachment);
    if (it != vec.end()) {
        vec.erase(it);
        return true;
    }
    return false;
}

template<VBOSpecifier S>
bool VAO::Attach(std::shared_ptr<VAOSubBuffer<S>> attachment, bool replace) {
    auto &vec = m_attachments.at(S);
    ecco_assert(std::find(vec.begin(), vec.end(), attachment) != vec.end(), "VAO::Attach - Can not attach non added attachment");

    auto set_slot = [&](int slot) {
        auto &cur = m_attachedAttachments[slot];
        if (cur.m_isAttached && !replace)
            return false;
        cur.m_isAttached = true;
        cur.m_attachment = attachment;
        cur.m_attachment->SetAttachmentSlot(slot);
        return true;
    };

    if (S == VBOSpecifier::VertexInfo)
        return set_slot(0);
    if (S == VBOSpecifier::NormalInfo)
        return set_slot(1);
    if (S == VBOSpecifier::TriangleInfo)
        return set_slot(2);
    if (S == VBOSpecifier::TexCoord1DInfo || S == VBOSpecifier::TexCoord2DInfo)
        return set_slot(3);

    int openSlot = -1;
    for (const auto &it : m_attachedAttachments)
        if (!it.second.m_isAttached && it.first > 3) { openSlot = it.first; break; }
    ecco_assert(openSlot != -1, "VAO::Attach - No available attachment slots");
    return set_slot(openSlot);
}


template<VBOSpecifier S>
bool VAO::Detach(std::shared_ptr<VAOSubBuffer<S>> attachment) {
    int slot = attachment->GetAttachmentSlot();
    if (slot < 0)
        return false;
    auto it = m_attachedAttachments.find(slot);
    if (it == m_attachedAttachments.end() || it->second.m_attachment != attachment)
        return false;
    it->second.m_isAttached = false;
    it->second.m_attachment = nullptr;
    attachment->SetAttachmentSlot(-1);
    return true;
}

void VAO::PrintAllAttachments() const {
    for (const auto& [spec, vec] : m_attachments) {
        if (vec.empty()) continue;
        std::cout << "ATTACHMENT TYPE: " << vec.front()->GetVBOName() << '\n';
        for (const auto& a : vec)  std::cout << a.get() << '\n';
    }
}

void VAO::PrintAttachedAttachments() const {
    for (const auto& [slot, info] : m_attachedAttachments) {
        if (!info.m_isAttached || !info.m_attachment) continue;
        std::cout << "SLOT " << slot
                  << " (" << info.m_attachment->GetVBOName() << ") -> "
                  << info.m_attachment.get() << '\n';
    }
}


template<VBOSpecifier S>
VAOSubBuffer<S>::VAOSubBuffer() : VAOSubBufferBase(), m_localSaved(false), m_numElements(0) { m_slotID = -1; }

template<VBOSpecifier S>
VAOSubBuffer<S>::~VAOSubBuffer() {}

template<VBOSpecifier S>
VBOSpecifier VAOSubBuffer<S>::GetVBOType() { return S; }

template<VBOSpecifier S>
const char* VAOSubBuffer<S>::GetVBOName() const { return VBOType<S>::m_vboName; };

template<VBOSpecifier S>
bool VAOSubBuffer<S>::GenerateBuffer() { return true; }

template<VBOSpecifier S>
bool VAOSubBuffer<S>::DeleteBuffer() { return true; }

template<VBOSpecifier S>
bool VAOSubBuffer<S>::BindBuffer() { return true; }

template<VBOSpecifier S>
bool VAOSubBuffer<S>::UnbindBuffer() { return true; }

template<VBOSpecifier S>
bool VAOSubBuffer<S>::SetData(const std::vector<DataType> &data, bool saveLocal) {
    auto vao = m_attachedVAO.lock();
    if (!vao)
        return false;
    vao->BindBuffer();
    this->BindBuffer();
    m_numElements = data.size();
    if (saveLocal) { m_localSaved = true; m_data = data; }
    this->UnbindBuffer();
    vao->UnbindBuffer();
    return true;
}

template<VBOSpecifier S>
void VAOSubBuffer<S>::ClearCache() { m_localSaved = false; m_data.clear(); }

template<VBOSpecifier S>
bool VAOSubBuffer<S>::SetVAO(std::shared_ptr<VAO> vao) { m_attachedVAO = vao; return true; }
