#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include "../ecco/EccoObject.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <string>
#include <tuple>

/**
 * store a weakptr to the parent/scenegraph so we dont have a cycle of shared_ptr's pointing at each other
 * and keeping each others reference counts alive forever
 *
 */

namespace ecco
{
    namespace SceneGraph
    {
        class SceneGraph;

        class SceneNode : public ecco::EccoObject,
                          public std::enable_shared_from_this<SceneNode> //to reference this shared_ptr

        {
        public:
            explicit SceneNode(std::string name);
            SceneNode() = delete;
            SceneNode(SceneNode&) = delete;
            ~SceneNode() = default;


            void AddChild(const std::shared_ptr<SceneNode>& child);
            bool RemoveChild(const std::shared_ptr<SceneNode>& child);

            void SetNodeToParentTransform(const glm::mat4& nodeToParent);
            glm::mat4 GetNodeToParentTransform() const;

            bool IsAncestorOf(const std::shared_ptr<SceneNode>& node) const;
            bool HasParent() const;

            /*
             * chosing to keep this returning a shared_ptr instead of weak because we will probably have
             * to call a bunch of stuff on the parent and i dont want to have to write .lock() everytime
             */
            std::shared_ptr<SceneNode> GetParentNode() const;
            std::shared_ptr<SceneNode> GetRootNode() const;

            glm::mat4 GetNodeToRootTransform() const;

            std::tuple<bool, glm::mat4> GetNodeToAncestorTransform(const std::shared_ptr<SceneNode>& child) const;

            const std::vector<std::shared_ptr<SceneNode>>& GetChildren() const { return m_children; }

            void SetSceneGraph(const std::shared_ptr<SceneGraph>& sceneGraph) { m_sceneGraph = sceneGraph; }

            /*
             * choosing this to return a weak ptr since a node will never really run any code on the parent
             * this will just be for debugging for the most part
             */
            std::weak_ptr<SceneGraph> GetSceneGraph() const { return m_sceneGraph; }

        private:

            glm::mat4 m_nodeToParentTransform;
            std::weak_ptr<SceneNode> m_parentNode;
            std::weak_ptr<SceneGraph> m_sceneGraph;
            std::vector<std::shared_ptr<SceneNode>> m_children;
        };
    }
}

#endif // SCENENODE_HPP
