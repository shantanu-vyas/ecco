#ifndef SCENEGRAPH_HPP
#define SCENEGRAPH_HPP

#include "../ecco/EccoObject.hpp"
#include "SceneNode.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <tuple>

namespace ecco {
namespace SceneGraph {
// forward declare to remove cyclical dependency
class SceneNode;

class SceneGraph : public ecco::EccoObject,
                   public std::enable_shared_from_this<SceneGraph> {
public:
  explicit SceneGraph(std::string name);
  SceneGraph() = delete;
  SceneGraph(const SceneGraph &) = delete;
  ~SceneGraph() = default;

  void SetRootNode(const std::shared_ptr<SceneNode> &rootNode);
  std::shared_ptr<SceneNode> GetRootNode() const;

  std::tuple<bool, glm::mat4>
  GetRootToNodeTransform(const std::shared_ptr<SceneNode> &node) const;

  std::tuple<bool, glm::mat4>
  GetNodeToNodeTransform(const std::shared_ptr<SceneNode> &n1,
                         const std::shared_ptr<SceneNode> &n2) const;

  void PrintTree(const std::shared_ptr<SceneNode> &start = nullptr,
                 int depth = 0) const;

private:
  std::shared_ptr<SceneNode> m_rootNode;
};
} // namespace SceneGraph
} // namespace ecco

#endif // SCENEGRAPH_HPP
