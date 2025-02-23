#ifndef MODEL_HPP
#define MODEL_HPP

#include "../ecco/EccoObject.hpp"
#include "../materials/Material.hpp"
#include "../utils/GLMUtils.hpp"
#include "../utils/assertMacros.hpp"
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

/**
 * Okay so last thing here is adding the VAO/VBO calls and how I want to manage this.
 * I had the idea for a VAOObject which is a wrapper on VAO/VBO/EBO/etc....
 * So a modelData should own a unique_ptr? to that VAOObject
 *
 * 1) Do we make 1 vao object for multiple submeshes? or do we make 1 vao object each
 * 2) When we call createVAO() we can not ALLOW the user to make it more than once so we keep track of that
 * 3) When we call createVAO() we can have an option to cache the vertices and triangle list and tex coord list of
 *    model, submesh, texmap but we should have it ideally clear these lists since we don't *need* to keep these in memory since
 *    they are now stored on the GPU. So we keep track of is isCached which is set to true in constructor and set to false depending on the
 *    flag passed into vaoObject = createVAO(bool deleteCache = true)
 *
 *
 * Okay side note: I might actually want a VAOManager because of issues where you can bind the wrong vbo to a vao?
 * Technically this shouldnt be a problem, doing
 * glBindVertexArray(vaoA);
 * glBindBuffer(GL_ARRA_BUFFER, vboB_vertices);
 * glDrawElements(GL_TRIANGLES ...) will call the draw on VAOA and all of A's VBO's not VBO B
 * VBO B would also be "replaced" into A if I manually did a glVertexAttribPointer
 * ... Maybe i dont need a manager for this... i did want managers to maintain all opengl stuff but not sure if its needed ahhhhhh
 */

/*
 * make a default model creator, for spheres, boxes, cones, rectangles etc.. basic shapes
 * make a stl loader (binary and ascii)
 *  */

namespace ecco {
    namespace Model {

        typedef std::vector<glm::dvec3> VertexList;
        typedef std::vector<glm::ivec3> TriangleList;
        typedef std::vector<glm::dvec2> TexCoords;

        static bool isVertexListSubset(const VertexList& a, const VertexList& b) {

            VertexList sortedSubset = a;
            VertexList sortedSuperset = b;

            std::sort(sortedSubset.begin(), sortedSubset.end(), glm::dvec3Sort);
            std::sort(sortedSuperset.begin(), sortedSuperset.end(), glm::dvec3Sort);

            return std::includes(sortedSuperset.begin(), sortedSuperset.end(),
                                 sortedSubset.begin(), sortedSubset.end(),
                                 glm::dvec3Sort);
        };

        static bool isVertexListEqual(const VertexList& v1, const VertexList& v2) {
            VertexList a = v1;
            VertexList b = v2;

            std::sort(a.begin(), a.end(), glm::dvec3Sort);
            std::sort(b.begin(), b.end(), glm::dvec3Sort);

            return a == b;
        }

        class Model : ecco::EccoObject,
                      public std::enable_shared_from_this<ecco::Model::Model> { //will be needed for making texture maps for the weakptr?

            public:
                Model() = delete;
                Model(Model &) = delete;

                Model(std::string name);
                ~Model() = default;
                void LoadModel(std::string filename);
                void SetTexCoords(const std::vector<glm::vec2> &coords);
                VertexList GetVertices() const { return m_vertices; };
                TriangleList GetTriangles() const { return m_triangles; };
                // idk what should be in here figure it out.
                // Different loaders?
            private:
                int m_numVertices;
                int m_numTriangles;
                VertexList m_vertices;
                TriangleList m_triangles;
        };

        class TextureMap : ecco::EccoObject {

            public:
                TextureMap() = delete;
                TextureMap(TextureMap &) = delete;
                TextureMap(const TextureMap &) = delete;

                TextureMap(std::string name,
                           std::shared_ptr<ecco::Model::Model> model,
                           TexCoords texCoords,
                           VertexList texVertex) :
                    ecco::EccoObject(name),
                    m_model(model),
                    m_texCoords(texCoords),
                    m_texVertex(texVertex) {

                    ValidateTexture();
                }

                void ValidateTexture() {
                    ecco_assert(m_texCoords.size() == m_texVertex.size(), "Texture Map Validate Error");
                    ecco_assert(isVertexListSubset(m_texVertex, m_model.lock()->GetVertices()), "Texture Map Validate Error");
                }

                VertexList GetVertices() const {
                    return m_texVertex;
                }

                TexCoords GetTexCoords() const {
                    return m_texCoords;
                }

            private:
                std::weak_ptr<Model> m_model;
                TexCoords m_texCoords;
                VertexList m_texVertex;

        };

        class SubMesh  : ecco::EccoObject {
            public:
                //Default Material Constructor
                SubMesh(std::string name,
                        std::shared_ptr<ecco::Model::Model> model,
                        VertexList vertices,
                        TriangleList triangles) :
                    ecco::EccoObject(name),
                    m_masterModel(model),
                    m_vertices(vertices),
                    m_triangles(triangles) {

                    ecco_assert(m_vertices.size() == m_masterModel.lock()->GetVertices().size(), "Submesh Constructor Error");

                }

                //Non Texture Mapped Material Constructor
                SubMesh(std::string name,
                        std::shared_ptr<ecco::Model::Model> model,
                        VertexList vertices,
                        TriangleList triangles,
                        std::shared_ptr<ecco::Material::Material> material) :
                    ecco::EccoObject(name),
                    m_masterModel(model),
                    m_vertices(vertices),
                    m_triangles(triangles),
                    m_material(material) {

                    ecco_assert(m_vertices == m_masterModel.lock()->GetVertices(), "Submesh Constructor Error");
                }

                //Texture Map Material Constructor
                SubMesh(std::string name,
                        std::shared_ptr<ecco::Model::Model> model,
                        VertexList vertices,
                        TriangleList triangles,
                        std::shared_ptr<ecco::Material::Material> material,
                        std::shared_ptr<ecco::Model::TextureMap> textureMap) :
                    ecco::EccoObject(name),
                    m_masterModel(model),
                    m_vertices(vertices),
                    m_triangles(triangles),
                    m_material(material),
                    m_textureMap(textureMap) {

                    ecco_assert(m_vertices == m_masterModel.lock()->GetVertices(), "Submesh Constructor Error");
                    ecco_assert(isVertexListEqual(m_vertices, m_textureMap->GetVertices()), "Submesh Constructor Error"); //Submesh vertices have to be 1 :1 with
                }


                VertexList GetVertices() const { return m_vertices; };
                TriangleList GetTriangles() const { return m_triangles; };



            private:
                std::weak_ptr<Model> m_masterModel;
                VertexList m_vertices;
                TriangleList m_triangles;
                std::shared_ptr<ecco::Material::Material> m_material;
                std::shared_ptr<ecco::Model::TextureMap> m_textureMap;
        };

    } // namespace Model
} // namespace ecco
#endif
