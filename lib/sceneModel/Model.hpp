#ifndef MODEL_HPP
#define MODEL_HPP

#include "../ecco/EccoObject.hpp"
#include "../materials/Material.hpp"
#include "../utils/GLMUtils.hpp"
#include "../utils/assertMacros.hpp"
#include "../utils/BBox.hpp"
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

        enum InternalData {
        VerticesID,
        TrianglesID,
        NormalsID,
        PerVertexColorsID,
        InstancingPositionsID,
        TexCoordsID
        };
        //Used to propogate changes to the vao model to update the gl internals
        using InternalDataCB = std::function<void(InternalData)>;


        using VertexList = std::vector<glm::dvec3>;
        using NormalList = std::vector<glm::dvec3>;
        using TriangleList = std::vector<glm::ivec3>;

        using InstancePositions = std::vector<glm::dvec3>;
        using PerVertexColors = std::vector<glm::dvec4>;
        using TexCoords = std::vector<glm::dvec2>;

        template <typename T>
        using PerVertexData = std::vector<T>;


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

        class Model : public ecco::EccoObject,
                      public std::enable_shared_from_this<ecco::Model::Model> { //will be needed for making texture maps for the weakptr?

            public:
                Model() = delete;
                Model(Model &) = delete;

                Model(std::string name);
                ~Model() = default;
                void LoadModel(std::string filename);

                //These functions will set the geometry and compute the bbox, (also first func will compute normals if they are not precomputed)
                //These need to trigger callbacks into the vao model
                void SetGeometry(const VertexList& vertexList, const TriangleList& triangleList); //computes normals
                void SetGeometry(const VertexList& vertexList, const TriangleList& triangleList, const NormalList& normalList);
                void SetInstancePositions(const InstancePositions& positions) { m_instancePositions = positions; };
                void SetPerVertexColors(const PerVertexColors& perVertexColors) { m_perVertexColors = perVertexColors; };
                void SetPerVertexTexCoords(const TexCoords texCoords) { m_texCoords = texCoords; };


                VertexList GetVertices() const { return m_vertices; };
                NormalList GetNormals() const { return m_normals; };
                TriangleList GetTriangles() const { return m_triangles; };
                InstancePositions GetInstancePositions() const { return m_instancePositions; };
                PerVertexColors GetPerVertexColors() const { return m_perVertexColors; };
                TexCoords GetPerVertexTexCoords() const { return m_texCoords; };

                BBox BBOX() const {
                    return m_bbox;
                }

                // idk what should be in here figure it out.
                // Different loaders?

            private:

                void computeNormals();

                int m_numVertices;
                int m_numTriangles;

                VertexList m_vertices;
                NormalList m_normals;
                TriangleList m_triangles;
                InstancePositions m_instancePositions;
                PerVertexColors m_perVertexColors;
                TexCoords m_texCoords;

                //ugh i do not like this, one model could have multiple vaos we don't define a strict 1:1 mapping
                //in which case if this class updates internal values we need to propgate multiple vaos to update
                //do we want to make this a vector? k
                //rather i think i want the VAOModel to have the callback member which binds to a function defined here called updateinternals
                //which looks at which internals got updated and propogates that change then this calls some internal functino in vaomodel
                //think about this later... but TODO
                InternalDataCB m_vaoUpdateCallback;

                BBox m_bbox;

        };

        class TextureMap : ecco::EccoObject {
            friend class SubMesh;
            public:
                TextureMap() = delete;
                TextureMap(TextureMap &) = delete;
                TextureMap(const TextureMap &) = delete;

                TextureMap(std::string name,
                           std::shared_ptr<ecco::Model::Model> model,
                           const TexCoords& texCoords,
                           size_t offset,
                           size_t size) :
                    ecco::EccoObject(name),
                    m_model(model),
                    m_texCoords(texCoords),
                    m_vertexOffset(offset),
                    m_vertexSize(size) {

                    validateTexture();
                }

                TexCoords GetTexCoords() const { return m_texCoords; };
                void SetTexCoords(const TexCoords& texCoords) {
                    m_texCoords = texCoords;
                    validateTexture();
                };

            private:
                void validateTexture() {
                    ecco_assert(m_texCoords.size() == m_vertexSize, "Texture Map Validate Error");
                }

                size_t m_vertexOffset;
                size_t m_vertexSize;
                std::weak_ptr<Model> m_model;
                TexCoords m_texCoords;


        };


        /*
         * Submeshes will have to be in the form of contigious triangles to make memory copies easier
         * Regardless if we have 1 or N submeshes we only want 1 vao with all the data 1 time otherwise
         *   a) we are requesting alot of resources
         *   b) we are binding alot of resources for a single object which is uneccesary if we can just do good bookkeeping
         *  */
        class SubMesh : ecco::EccoObject {

            public:
                //Default Material Constructor
                SubMesh(std::string name,
                        std::shared_ptr<ecco::Model::Model> model,
                        size_t vertexOffset,
                        size_t vertexSize) :
                    ecco::EccoObject(name),
                    m_masterModel(model),
                    m_vertexOffset(vertexOffset),
                    m_vertexSize(vertexSize) {

                    ecco_assert(areVertexBoundsValid(), "Submesh Constructor Error");

                }

                //Non Texture Mapped Material Constructor
                SubMesh(std::string name,
                        std::shared_ptr<ecco::Model::Model> model,
                        size_t vertexOffset,
                        size_t vertexSize,
                        std::shared_ptr<ecco::Material::Material> material) :
                    ecco::EccoObject(name),
                    m_masterModel(model),
                    m_vertexOffset(vertexOffset),
                    m_vertexSize(vertexSize),
                    m_material(material) {

                    ecco_assert(areVertexBoundsValid(), "Submesh Constructor Error");
                }

                //Texture Map Material Constructor
                SubMesh(std::string name,
                        std::shared_ptr<ecco::Model::Model> model,
                        size_t vertexOffset,
                        size_t vertexSize,
                        std::shared_ptr<ecco::Material::Material> material,
                        std::shared_ptr<ecco::Model::TextureMap> textureMap) :
                    ecco::EccoObject(name),
                    m_masterModel(model),
                    m_vertexOffset(vertexOffset),
                    m_vertexSize(vertexSize),
                    m_material(material),
                    m_textureMap(textureMap) {

                    ecco_assert(areVertexBoundsValid(), "Submesh Constructor Error");

                    //These will probably eventually be not needed since we will create the texturemap given a submesh param
                    //leaving it in for now until i start writing that part
                    ecco_assert(m_vertexOffset != m_textureMap->m_vertexOffset, "Submesh Constructor Error");
                    ecco_assert(m_vertexSize != m_textureMap->m_vertexSize, "Submesh Constructor Error");
                }

                std::shared_ptr<ecco::Material::Material> GetMaterial() const {
                    return m_material;
                }

                std::shared_ptr<ecco::Model::TextureMap> GetTextureMap() const {
                    return m_textureMap;
                }

                BBox BBOX() const {
                    return m_bbox;
                }

            private:
                //having this return bool instead of asserting directly so assert gives us line number for which constructor
                bool areVertexBoundsValid() {
                    if (m_vertexOffset > m_masterModel.lock()->GetVertices().size() ||
                        m_vertexOffset + m_vertexSize > m_masterModel.lock()->GetVertices().size())
                        return false;
                    return true;
                }

                std::weak_ptr<Model> m_masterModel;
                size_t m_vertexOffset;
                size_t m_vertexSize;
                std::shared_ptr<ecco::Material::Material> m_material;
                std::shared_ptr<ecco::Model::TextureMap> m_textureMap;
                BBox m_bbox;

        };

    } // namespace Model
} // namespace ecco
#endif
