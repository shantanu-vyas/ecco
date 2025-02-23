#ifndef VAOMODEL_HPP
#define VAOMODEL_HPP

#include <GL/gl.h>
#include "../ecco/EccoProduct.hpp"
#include "../sceneModel/Model.hpp"
#include <memory>


/*
* TODO
* 1) does 1 vao model have all submeshes
* 2) constructors need to pull out material data from the model to determine how to set up the VAO
*    if model has a material, if material has texture coords make tex coord stuff
*    if model has a material, if model has no texture coords ...
*    if model has a material, multiple texmapps ..etc....
*    gotta figure this out some other time
*
*
*/


namespace ecco
{
    namespace OpenGL
    {
        class VAOModel : public ecco::EccoProduct
        {
            public:
                VAOModel() = delete;
                VAOModel(VAOModel&) = delete;
                VAOModel(const VAOModel&) = delete;

                //Create basic VAO model
                VAOModel(std::shared_ptr<ecco::Model::Model> model);

                VAOModel(std::shared_ptr<ecco::Model::Model> model,
                         ecco::Model::InstancePositions instancingPositions);
                VAOModel(std::shared_ptr<ecco::Model::Model> model,
                         ecco::Model::PerVertexColors perVertexColors);

                VAOModel(std::shared_ptr<ecco::Model::Model> model,
                         ecco::Model::InstancePositions instancingPositions,
                         ecco::Model::PerVertexColors perVertexColors);

                void EnableInstancing(); //assert if m_hasInstancing is false
                void DisableInstancing();
                bool GetHasInstancing() const;
                bool GetIsInstancing() const;

                void EnablePerVertexColors(); //assert if m_hasPerVertexColors is false
                void DisablePerVertexColors();
                bool GetHasPerVertexColors() const;
                bool GetIsPerVertexColors() const;

                void CreateVAO(std::shared_ptr<Model::Model> model);

            private:
                GLuint m_vao, m_vboVerts, m_vboNorms, m_eboTriangles, m_vboTexCoords;
                int m_numVertices, m_numTriangles;

                bool m_hasInstancing;
                bool m_isInstancing;
                GLuint m_vaoInstancedPositions;

                bool m_hasPerVertexColors;
                bool m_isPerVertexColors;
                GLuint m_perVertexColors;

                //Model owns shared ptr to this object
                std::weak_ptr<ecco::Model::Model> m_model;
                std::weak_ptr<ecco::Material::Material> m_material; //Get material handle here


                //texture link

                bool m_hasFrequentUpdates; //Set to true if this object is updated every frame~ this will be to set dynamic draw in gl

        };
    }
}

#endif
