#include "SceneGraph.hpp"
#include "SceneNode.hpp"
#include "../utils/assertMacros.hpp"
#include <iostream>
#include <functional>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ecco::SceneGraph;

SceneGraph::SceneGraph(std::string name)
    : EccoObject(name)
    , m_rootNode(nullptr) {

}

void SceneGraph::SetRootNode(const std::shared_ptr<SceneNode> rootNode)
{
    ecco_assert(rootNode != nullptr, "Set Root Node Error");

    m_rootNode = rootNode;

    if (m_rootNode)
    {
        //All children of root node now need to point at the correct scenegraph
        //idk if the child actually needs a ref to scene graph but it might be useful for debugging scenes
        std::function<void(const std::shared_ptr<SceneNode>&)> setGraphRecursively;
        setGraphRecursively = [&](const std::shared_ptr<SceneNode>& node) {

            if (!node) return;
            node->SetSceneGraph(shared_from_this());

            for (auto& child : node->GetChildren()) {
                setGraphRecursively(child);
            }
        };

        setGraphRecursively(m_rootNode);
    }
}

std::shared_ptr<SceneNode> SceneGraph::GetRootNode() const {

    return m_rootNode;
}

std::tuple<bool, glm::mat4> SceneGraph::GetRootToNodeTransform(const std::shared_ptr<SceneNode> node) const
{
    //if root node or node is null return false
    if (!m_rootNode || !node) {
        return { false, glm::mat4(1.0f) };
    }

    //make sure the node exists in the current scene graph
    auto nodeGraph = node->GetSceneGraph().lock();
    if (nodeGraph.get() != this) {
        return { false, glm::mat4(1.0f) };
    }

    glm::mat4 nodeToRoot = node->GetNodeToRootTransform();
    glm::mat4 rootToNode = glm::inverse(nodeToRoot);

    return { true, rootToNode };
}

std::tuple<bool, glm::mat4> SceneGraph::GetNodeToNodeTransform(const std::shared_ptr<SceneNode> n1,
                                                               const std::shared_ptr<SceneNode> n2) const {

    if (!m_rootNode || !n1 || !n2) {
        return { false, glm::mat4(1.0f) };
    }

    //make sure both nodes exist in this scene graph
    auto g1 = n1->GetSceneGraph().lock();
    auto g2 = n2->GetSceneGraph().lock();
    if (g1.get() != this || g2.get() != this) {
        return { false, glm::mat4(1.0f) };
    }


    glm::mat4 n1ToRoot = n1->GetNodeToRootTransform();
    glm::mat4 n2ToRoot = n2->GetNodeToRootTransform();

    glm::mat4 n1ToN2 = n2ToRoot * glm::inverse(n1ToRoot);

    return { true, n1ToN2 };
}

void SceneGraph::PrintTree(const std::shared_ptr<SceneNode> start, int depth) const {
    std::shared_ptr<SceneNode> node = start ? start : m_rootNode;

    if (!node) {
        std::cout << "(SceneGraph) No root node set, nothing to print.\n";
        return;
    }
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    std::cout << node->GetName() << std::endl;

    for (auto& child : node->GetChildren()) {
        PrintTree(child, depth + 1);
    }
}
