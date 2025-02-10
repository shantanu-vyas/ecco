#ifndef SCENEGRAPH_HPP
#define SCENEGRAPH_HPP

#include "SceneNode.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>

namespace ecco
{
    namespace SceneGraph
    {
        class SceneNode;
        //needs to set the root node so it has its m_sceneGraph set to this
        //should we pass smart pointers by reference?
        class SceneGraph
        {
            friend class SceneNode;
            public:
                explicit SceneGraph(std::string name);
                SceneGraph() = delete;
                SceneGraph(SceneGraph&) = delete;
                ~SceneGraph();

                void SetRootNode(std::shared_ptr<ecco::SceneGraph::SceneNode> rootNode);
                std::shared_ptr<ecco::SceneGraph::SceneNode> GetRootNode() const;
                glm::mat4 GetRootToNodeTransform(std::shared_ptr<ecco::SceneGraph::SceneNode> node) const; //return on fail?
                glm::mat4 GetNodeToNodeTransform(std::shared_ptr<ecco::SceneGraph::SceneNode> n1, std::shared_ptr<ecco::SceneGraph::SceneNode> n2) const;
            private:
                const std::shared_ptr<ecco::SceneGraph::SceneNode> m_rootNode;
                std::vector<std::shared_ptr<const ecco::SceneGraph::SceneNode>> m_allChildren;
        };
    }
}

#endif //SCENEGRAPH_HPP
