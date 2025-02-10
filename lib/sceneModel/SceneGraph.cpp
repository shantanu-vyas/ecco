#include "SceneGraph.hpp"

using ecco::SceneGraph::SceneGraph;
using ecco::SceneGraph::SceneNode;;

SceneGraph::SceneGraph(std::string name)
: EccoObject(name)
{

}
SceneGraph::~SceneGraph() { }
void SceneGraph::SetRootNode(std::shared_ptr<SceneNode> rootNode) {}
std::shared_ptr<SceneNode> SceneGraph::GetRootNode() const {}
glm::mat4 SceneGraph::GetRootToNodeTransform(std::shared_ptr<SceneNode> node) const {} //return on fail?
glm::mat4 SceneGraph::GetNodeToNodeTransform(std::shared_ptr<SceneNode> n1, std::shared_ptr<SceneNode> n2) const {}
void PrintTree(const std::shared_ptr<SceneNode> from = nullptr) {};
