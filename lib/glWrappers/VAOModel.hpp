#ifndef VAOMODEL_HPP
#define VAOMODEL_HPP

#include <GL/gl.h>
#include "../ecco/EccoProduct.hpp"
#include "../sceneModel/Model.hpp"
#include <memory>


//Vao Object shouldnt link against a texture it should linke against a material
//material has -> tex id, albedo, diffuse, specular, ambient, shinieness, roughness, alpha, per vertex colors, per face color, gi settings etc.... shadows blah blah
//material also needs a way to chose which coloring mode its using (p vertex color, phong shading, p triangle colors)
//material also needs to flag if its enabling post processing effects, gi, shadows, w/e


namespace ecco
{
    namespace OpenGL
    {
        class VAOModel : public ecco::EccoProduct
        {
            public:
                // explicit VAOModel(const std::shared_ptr<Model::Model> model);
                // VAOModel() = delete;
                // VAOModel(VAOModel&) = delete;
                // ~VAOModel();


                void RecreateVAO(std::shared_ptr<Model::Model> model);
                void LinkTexture(); //Texture object isnt created yet
                void DeleteVao();

            private:
                GLuint m_vao, m_vboVerts, m_vboNorms, m_vboTexCoords;

                bool m_instancedObjects;
                GLuint m_vaoInstancedPositions;

                //texture link

                bool m_hasFrequentUpdates; //Set to true if this object is updated every frame~ this will be to set dynamic draw in gl

        };
    }
}

#endif
