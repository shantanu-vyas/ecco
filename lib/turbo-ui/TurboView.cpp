#include "TurboView.hpp"
#include "../utils/assertMacros.hpp"
#include "ecco/EccoObject.hpp"

using namespace ecco::Turbo;

TurboView::TurboView(std::string name) :
    EccoObject(name),
    m_bounds(0,0,1,1),
    m_margins(0,0,0,0),
    m_padding(0,0,0,0),
    m_parent(std::weak_ptr<TurboView>()),
    m_children(),
    m_frameTime(0) {
}

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

/* How is the call order here going to work
 * PrePreRender gets called on all views that have it before anything
 * PreRender gets called right before a view is rendered
 * Render gets called to render the view
 * PostRender gets called right after a view is rendered
 * PostPostRender gets called on all view after everything has been rendered
 * */

void TurboView::PrePreRender() {
    OnPrePreRender();
    for (auto child : m_children)
        child->PrePreRender();
}
void TurboView::OnPrePreRender() {
    //Child writes this
}
void TurboView::OnPreRender() {
    //Child writes this
}
void TurboView::Render() {
    OnPreRender();
    OnRender();
    OnPostRender();
    for (auto child : m_children) {
        child->OnPrePreRender();
        child->Render();
        child->OnPostPostRender();
    }
}
void TurboView::OnRender() {
    //Child writes this
}
void TurboView::OnPostRender() {
    //Child writes this
}
void TurboView::PostPostRender() {
    OnPostPostRender();
    for (auto child : m_children) {
        child->PostPostRender();
    }
}
void TurboView::OnPostPostRender() {
    //child writes this
}

//Called by appdelegate in loop
void TurboView::RenderAll() {
    //do we need to worry aobut thread safety here? if so we need to lock children
    PrePreRender(); //PrePreRender on all elements
    Render(); //PreRender -> Render -> Post Render per element
    PostPostRender(); //PostPostRender on all elements
}

void TurboView::AddChild(const std::shared_ptr<TurboView> child) {
    ecco_assert(child != nullptr, "TurboView::AddChild : can not add nullptr child");
    ecco_assert(child->HasParent(), "TurboView::AddChild : can not add child with parent");

    m_children.emplace_back(child);
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

void TurboView::PrintTree(int depth) {
    for (size_t i = 0; i < depth; ++i)
        std::cout << "  ";

    std::cout << GetName() << std::endl;
    for (auto child : m_children)
        child->PrintTree(depth+1);
}
