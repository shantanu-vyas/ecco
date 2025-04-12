#include "GLBuffers.hpp"

VAO::VAO() {}
VAO::~VAO() {}
bool VAO::GenerateBuffer() {}
bool VAO::DeleteBuffer() {}
bool VAO::BindBuffer() {}
bool VAO::UnbindBuffer() {}
bool VAO::SetAttachment(const VBOType &type,
                        std::unique_ptr<VBOType> attachment) {}
bool VAO::RemoveAttachment(const VBOType &type,
                           std::unique_ptr<VBOType> attachment) {}
std::unique_ptr<VBOType> &&
VAO::MoveAttachment(const VBOType &type, std::unique_ptr<VBOType> attachment) {}

VAOSubBuffer::VAOSubBuffer() {}
VAOSubBuffer::~VAOSubBuffer() {}
bool VAOSubBuffer::GenerateBuffer() {}
bool VAOSubBuffer::DeleteBuffer() {}
bool VAOSubBuffer::BindBuffer() {}
bool VAOSubBuffer::UnbindBuffer() {}
T VAOSubBuffer::GetType() const {}
