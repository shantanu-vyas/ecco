#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>

/*
 * Notes
 * should i be storing parentTnode instead of nodeTparent? i think so
 * how tf do i pass these shared_ptrs around correctly i want them to be const here does that mean i have a
 * shared_ptr<const Scenenode>? this is a different type from the shared ptr thats passed in
 *
 *  */

namespace ecco
{
    namespace SceneGraph
    {
        class SceneGraph;
        class SceneNode
        {
            public:
                explicit SceneNode(std::string name);
                SceneNode() = delete;
                SceneNode(SceneNode&) = delete;
                ~SceneNode();

                /* needs to add node to this as a child and update master ref list in scene graph */
                bool AddChild(const std::shared_ptr<SceneNode> child);
                bool RemoveChild(const std::shared_ptr<SceneNode> child);

                void SetNodeToParentTransform(const glm::mat4& nodeTparent);
                glm::mat4 GetNodeToParentTransform() const;

                const std::shared_ptr<SceneNode> GetParentNode();
                const std::shared_ptr<SceneNode> GetRootNode();

                glm::mat4 GetNodeTRootTransform() const;
                glm::mat4 GetNodeTParentTransform() const;
                std::tuple<bool, glm::mat4> GetNodeTAncestorTransform(const std::shared_ptr<SceneNode> child) const; //if it exists, value this could be replaced with an outcome

            private:
                glm::mat4 m_nodeTparentTransform();
                const std::shared_ptr<SceneNode> m_parentNode;
                const std::shared_ptr<SceneNode> m_rootNode;
                const std::shared_ptr<ecco::SceneGraph::SceneGraph> m_sceneGraph;
                //this cant be const in a vector?
                std::vector<std::shared_ptr<const SceneNode>> m_children;
        };
    }
}
#endif //SCENENODE_HPP
