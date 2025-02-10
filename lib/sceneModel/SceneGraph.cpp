#include "SceneGraph.hpp"

using ecco::SceneGraph::SceneGraph;

SceneGraph::SceneGraph(std::string name) {

}
SceneGraph::~SceneGraph() { }
void SceneGraph::SetRootNode(std::shared_ptr<ecco::SceneGraph::SceneNode> rootNode) {}
std::shared_ptr<ecco::SceneGraph::SceneNode> SceneGraph::GetRootNode() const {}
glm::mat4 SceneGraph::GetRootToNodeTransform(std::shared_ptr<ecco::SceneGraph::SceneNode> node) const {} //return on fail?
glm::mat4 SceneGraph::GetNodeToNodeTransform(std::shared_ptr<ecco::SceneGraph::SceneNode> n1, std::shared_ptr<ecco::SceneGraph::SceneNode> n2) const {}
