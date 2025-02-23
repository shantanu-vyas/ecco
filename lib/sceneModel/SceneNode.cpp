#include "SceneNode.hpp"
#include "../utils/assertMacros.hpp"
#include <algorithm>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ecco::SceneGraph;
using namespace ecco::Model;

SceneNode::SceneNode(std::string name) :
    EccoObject(name),
    m_nodeToParentTransform(1.0f) {

}

void SceneNode::AddChild(const std::shared_ptr<SceneNode>& child) {

    if (!child) {
        return;
    }

    ecco_assert(!child->HasParent(), "Child already has a parent");

    //Do not do = new shared_ptr<scenenode> this or we create a new one
    //if we use shared_from_this() this new smart pointer is added to the same ownership block as whoever owns this
    child->m_parentNode = shared_from_this();
    m_children.push_back(child);
}

bool SceneNode::RemoveChild(const std::shared_ptr<SceneNode>& child) {
    if (!child) {
        return false;
    }

    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        child->m_parentNode.reset(); //reset the childs weakptr ref to this
        m_children.erase(it);
        return true;
    }
    return false;
}

void SceneNode::SetNodeToParentTransform(const glm::mat4& nodeToParent) {

    m_nodeToParentTransform = nodeToParent;
}

glm::mat4 SceneNode::GetNodeToParentTransform() const {

    return m_nodeToParentTransform;
}

bool SceneNode::IsAncestorOf(const std::shared_ptr<SceneNode>& node) const {

    if (!node) {
        return false;
    }

    std::shared_ptr<SceneNode> current = node;
    while (std::shared_ptr<SceneNode> parent = current->GetParentNode()) {
        if (parent.get() == this) {
            return true;
        }
        current = parent;
    }
    return false;
}

bool SceneNode::HasParent() const {

    /*
     *  weakptrs' validity must be checked with expired
     *  expired is true if never set or the sharedptr its pointing to dies or was never init
     *  */
    return !m_parentNode.expired();
}

std::shared_ptr<SceneNode> SceneNode::GetParentNode() const {

    /*
     * lock will attempt to create shared_ptr if m_parentNode(weakptr) is not expired otherwise it returns nullptr
     */
    return m_parentNode.lock();
}

std::shared_ptr<SceneNode> SceneNode::GetRootNode() const {


    std::shared_ptr<SceneNode> current = const_cast<SceneNode*>(this)->shared_from_this();
    while (current->HasParent()) {
        current = current->GetParentNode();
    }
    return current;
}

glm::mat4 SceneNode::GetNodeToRootTransform() const {

    // Accumulate transforms up the chain
    glm::mat4 transform(1.0f);
    std::shared_ptr<SceneNode> current = const_cast<SceneNode*>(this)->shared_from_this();
    while (current) {
        transform = current->m_nodeToParentTransform * transform;
        current = current->GetParentNode();
    }
    return transform;
}

/* this is where the outcome class would be useful */
std::tuple<bool, glm::mat4> SceneNode::GetNodeToAncestorTransform(const std::shared_ptr<SceneNode>& child) const {

    if (!child || !IsAncestorOf(child)) {
        return { false, glm::mat4(1.0f) };
    }

    // T(this->child) = T(nodeToRoot)_child * inverse(T(nodeToRoot)_this)
    glm::mat4 childToRoot    = child->GetNodeToRootTransform();
    glm::mat4 ancestorToRoot = GetNodeToRootTransform();
    glm::mat4 transform      = childToRoot * glm::inverse(ancestorToRoot);

    return { true, transform };
}

bool SceneNode::AddModelToNode(const std::shared_ptr<ecco::Model::Model>& model) {
    if (std::find(m_models.begin(), m_models.end(), model) != m_models.end())
        return false; //model exists already

    m_models.emplace_back(model);
    return true;
}

bool SceneNode::RemoveModelFromNode(const std::shared_ptr<ecco::Model::Model>& model) {
    auto it = std::find(m_models.begin(), m_models.end(), model);
    if (it == m_models.end()) //model doesnt exist
        return false;

    m_models.erase(it);
    return true;

}

const std::vector<std::shared_ptr<Model>>& SceneNode::GetAllModelsFromNode() {
    return m_models;
}
