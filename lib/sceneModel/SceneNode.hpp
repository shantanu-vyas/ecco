#include <glm.hpp>
#include <memory.hpp>
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
        class SceneNode
        {
            public:
                SceneNode(std::string name) explicit;
                SceneNode() = delete;
                SceneNode(SceneNode) = delete;
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
                const std::shared_ptr<SceneGraph> m_sceneGraph;
                std::vector<const std::shared_ptr<SceneNode>> m_children;
        }
    }
}
