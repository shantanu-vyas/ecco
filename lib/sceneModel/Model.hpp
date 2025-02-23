#ifndef MODEL_HPP
#define MODEL_HPP

#include "../ecco/EccoObject.hpp"
#include <algorithm> // std::sort, std::includes
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <memory>

/*
 * eh fook i wrote out this header then remembered my idea for meshes where we break them into submeshes
 * a mesh can have 1-N submeshes, each of these submeshes is esentially just a mesh but owns its own Material
 * This way a single mesh can have multiple different materials for different components
 * ex. Mario's eyes can be a different texture from his faces but the hat and body can be part of the same model
 *  */


namespace ecco {
    namespace Model {

        class Model : ecco::EccoObject,
                      public std::enable_shared_from_this<SceneNode> { //will be needed for making texture maps for the weakptr?

            public:
                Model() = delete;
                Model(Model &) = delete;

                Model(std::string name);
                ~Model() = default;
                void LoadModel(std::string filename);
                void SetTexCoords(const std::vector<glm::vec2> &coords);
                std::vector<glm::dvec3> GetVertices() const { return m_vertices; };
                std::vector<glm::ivec3> GetTriangles() const { return m_triangles; };
                // idk what should be in here figure it out.
                // Different loaders?
            private:
                int m_numVertices;
                int m_numTriangles;
                std::vector<glm::dvec3> m_vertices;
                std::vector<glm::ivec3> m_triangles;
        };

        class SubMesh() {
            //weakptr ref to model
            //also needs access to glmvecsort and issubset so we will just move those to general util functions for now
            //has a subset list of triangles and vertices
            //has a weak reference to a material
            //has a texturemap
        }

        class TextureMap {
            TextureMap() = delete;
            TextureMap(TextureMap &) = delete;
            TextureMap(const TextureMap &) = delete;

            TextureMap(std::shared_ptr<ecco::Model::Model> model,
                       std::vector<glm::dvec2> texCoords,
                       std::vector<glm::dvec3> texVertex)
                : m_model(model), m_texCoords(texCoords), m_texVertex(texVertex) {

                ValidateTexture();
            }

            void ValidateTexture() {
                assert(m_texCoords.size() == m_texVertex.size());

                //write a glm sort for now
                auto glmVecSort = [](const glm::dvec3 &a, const glm::dvec3 &b) {
                    if (a.x != b.x)
                        return a.x < b.x;
                    if (a.y != b.y)
                        return a.y < b.y;
                    return a.z < b.z;
                };

                //this doesn't need to be a lambda but i wanna play w breaking up big functions
                auto isSubset = [&, this]() -> bool {

                    std::vector<glm::dvec3> sortedSubset = m_texVertex;
                    std::vector<glm::dvec3> sortedSuperset = m_model.lock()->GetVertices();


                    std::sort(sortedSubset.begin(), sortedSubset.end(), glmVecSort);
                    std::sort(sortedSuperset.begin(), sortedSuperset.end(), glmVecSort);

                    return std::includes(sortedSuperset.begin(), sortedSuperset.end(),
                                         sortedSubset.begin(), sortedSubset.end(),
                                         glmVecSort);
                };

                assert(isSubset());
            }
            private:
                std::weak_ptr<Model> m_model;
                std::vector<glm::dvec2> m_texCoords;
                std::vector<glm::dvec3> m_texVertex;

        };
    } // namespace Model
} // namespace ecco
#endif
