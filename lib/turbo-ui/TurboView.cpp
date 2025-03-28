#include "TurboView.hpp"
#include "../utils/assertMacros.hpp"

using namespace ecco::Turbo;

void TurboView::SetFrame(const Rect &rect) {
    m_bounds = rect;
    if (m_onResizeCB)
        m_onResizeCB();
}
void TurboView::SetCenter(const glm::dvec2 &center) {
    m_bounds.m_center = center;
}
void TurboView::SetX(float x) {
    m_bounds.m_center.x = x;
}
void TurboView::SetY(float y) {
    m_bounds.m_center.y = y;
}
void TurboView::SetSize(const glm::dvec2 &size) {
    m_bounds.m_size = size;
    if (m_onResizeCB)
        m_onResizeCB();
}
void TurboView::SetWidth(float w) {
    m_bounds.m_size.x = w;
    if (m_onResizeCB)
        m_onResizeCB();
}
void TurboView::SetHeight(float h) {
    m_bounds.m_size.y = h;
    if (m_onResizeCB)
        m_onResizeCB();
}
void TurboView::AddChild(const std::shared_ptr<TurboView> child) {
    ecco_assert(child == nullptr, "TurboView::AddChild : can not add nullptr child");
    ecco_assert(child->HasParent(), "TurboView::AddChild : can not add child with parent");

    m_children->emplace_back(child);
}

bool TurboView::HasParent() {
    return m_parent.lock() == nullptr;
}
std::weak_ptr<TurboView> TurboView::GetParent() {
    return m_parent.lock();
}
bool TurboView::IsRoot() {
    return false;
}
void TurboView::SetOnResizeCBFunction(const std::function<void(void)> &func) {
    m_onResizeCB = func;
}
