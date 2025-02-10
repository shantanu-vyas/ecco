#include "SceneNode.hpp"
#include <vector>
namespace ecco
{
    namespace SceneGraph
    {
        //needs to set the root node so it has its m_sceneGraph set to this
        //should we pass smart pointers by reference?
        friend class SceneGraph;
        class SceneGraph
        {
            public:
                SceneGraph(std::string name) explicit;
                SceneGraph() = delete;
                SceneGraph(SceneGraph) = delete;
                ~SceneGraph();

                void SetRootNode(std::shared_ptr<SceneNode> rootNode);
                std::shared_ptr<SceneNode> GetRootNode() const;
                glm::mat4 GetRootToNodeTransform(std::shared_ptr<SceneNode> node) const; //return on fail?
                glm::mat4 GetNodeToNodeTransform(std::shared_ptr<SceneNode> n1, std::shared_ptr<SceneNode> n2) const;
            private:
                const std::shared_ptr<SceneNode> m_rootNode;
                std::vector<const std::shared_ptr<SceneNode>> m_allChildren;
        }
    }
}
