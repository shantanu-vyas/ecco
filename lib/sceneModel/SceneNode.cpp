#include "SceneNode.hpp"


using ecco::SceneGraph::SceneNode;


ecco::SceneGraph::SceneNode(std::string name) :

{

}

bool SceneNode::AddChild(const std::shared_ptr<SceneNode> child) { }
bool SceneNode::RemoveChild(const std::shared_ptr<SceneNode> child) { }
void SceneNode::SetNodeToParentTransform(const glm::mat4& nodeTparent) { }
glm::mat4 SceneNode::GetNodeToParentTransform() const {}
const std::shared_ptr<SceneNode> SceneNode::GetParentNode() { }
const std::shared_ptr<SceneNode> SceneNode::GetRootNode() { }
glm::mat4 SceneNode::GetNodeTRootTransform() const { }
glm::mat4 SceneNode::GetNodeTParentTransform() const { }
std::tuple<bool, glm::mat4> SceneNode::GetNodeTAncestorTransform(const std::shared_ptr<SceneNode> child) const { }
