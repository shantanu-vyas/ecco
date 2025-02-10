#ifndef SCENEGRAPH_HPP
#define SCENEGRAPH_HPP

#include "../ecco/EccoObject.hpp"
#include "SceneNode.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>


//after i made everything inherit from ecco::ecco::EccoObject i had to change the namespaces of params from SceneGraph::SceneNode to just SceneNode?

namespace ecco
{
    namespace SceneGraph
    {
        class SceneNode;
        //needs to set the root node so it has its m_sceneGraph set to this
        //should we pass smart pointers by reference?
        class SceneGraph : ecco::EccoObject
        {
            friend class SceneNode;
            public:
                explicit SceneGraph(std::string name);
                SceneGraph() = delete;
                SceneGraph(SceneGraph&) = delete;
                ~SceneGraph();

                void SetRootNode(std::shared_ptr<SceneNode> rootNode);
                std::shared_ptr<SceneNode> GetRootNode() const;
                glm::mat4 GetRootToNodeTransform(std::shared_ptr<SceneNode> node) const; //return on fail?
                glm::mat4 GetNodeToNodeTransform(std::shared_ptr<SceneNode> n1, std::shared_ptr<SceneNode> n2) const;

                void PrintTree(const std::shared_ptr<SceneNode> from = nullptr);

            private:
                const std::shared_ptr<SceneNode> m_rootNode;
                std::vector<std::shared_ptr<const SceneNode>> m_allChildren;
        };
    }
}

#endif //SCENEGRAPH_HPP
