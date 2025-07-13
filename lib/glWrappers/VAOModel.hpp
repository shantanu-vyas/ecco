#ifndef VAOMODEL_HPP
#define VAOMODEL_HPP

#include <GL/glew.h>
#include <GL/gl.h>
#include "../sceneModel/Model.hpp"
#include "GLTypes.hpp"


#include <memory>


/*
* TODO
* 1) does 1 vao model have all submeshes
*    1 vao would be good since its less data overhead
* 2) constructors need to pull out material data from the model to determine how to set up the VAO
*    if model has a material, if material has texture coords make tex coord stuff
*    if model has a material, if model has no texture coords ...
*    if model has a material, multiple texmapps ..etc....
*    gotta figure this out some other time
*/

// -----------------------------------------
// C1: Default material (no per-vertex color, no texture map)
// -----------------------------------------

// C11: Just a model (default material)
       // VAO, VBO, VBONorm, EBO

// C12: Model + instancing (default material)
       // VAO, VBO, VBONorm, EBO, InstancingPositions


// -----------------------------------------
// C2: Per-vertex color (with or without UV, with or without instancing)
// -----------------------------------------

// C21: Model + per vertex colors
       // VAO, VBO, VBONorm, EBO, PerVertexColor

// C22: Model + instancing + per vertex colors
       // VAO, VBO, VBONorm, EBO, InstancingPositions, PerVertexColor

// C23: Model + per vertex colors + UV coords
       // VAO, VBO, VBONorm, EBO, PerVertexColor, UVCoords

// C24: Model + instancing + per vertex colors + UV coords
       // VAO, VBO, VBONorm, EBO, InstancingPositions, PerVertexColor, UVCoords


// -----------------------------------------
// C3: Texture map (UV coords), possibly combined with per-vertex color
// -----------------------------------------

// C31: Model + texture map (UV coords)
       // VAO, VBO, VBONorm, EBO, UVCoords

// C32: Model + instancing + texture map (UV coords)
       // VAO, VBO, VBONorm, EBO, InstancingPositions, UVCoords

// C33: Model + texture map + per vertex colors
       // VAO, VBO, VBONorm, EBO, UVCoords, PerVertexColor

// C34: Model + instancing + texture map + per vertex colors
       // VAO, VBO, VBONorm, EBO, InstancingPositions, UVCoords, PerVertexColor




namespace ecco
{
    namespace OpenGL
    {
        class VAOModel : public ecco::EccoObject
        {

            public:
                VAOModel() = delete;
                VAOModel(VAOModel&) = delete;
                VAOModel(const VAOModel&) = delete;

                VAOModel(const std::shared_ptr<ecco::Model::Model>& model);

                VAOModel(const std::shared_ptr<ecco::Model::Model>& model,
                         const ecco::Model::InstancePositions& instancingPositions);


                VAOModel(const std::shared_ptr<ecco::Model::Model>& model,
                         const ecco::Model::PerVertexColors& perVertexColors);


                VAOModel(const std::shared_ptr<ecco::Model::Model>& model,
                         const ecco::Model::InstancePositions& instancingPositions,
                         const ecco::Model::PerVertexColors& perVertexColors);

                VAOModel(const std::shared_ptr<ecco::Model::Model>& model,
                         const ecco::Model::TexCoords& uvCoords);


                VAOModel(const std::shared_ptr<ecco::Model::Model>& model,
                         const ecco::Model::InstancePositions& instancingPositions,
                         const ecco::Model::TexCoords& uvCoords);

                VAOModel(const std::shared_ptr<ecco::Model::Model>& model,
                         const ecco::Model::PerVertexColors& perVertexColors,
                         const ecco::Model::TexCoords& uvCoords);

                VAOModel(const std::shared_ptr<ecco::Model::Model>& model,
                         const ecco::Model::InstancePositions& instancingPositions,
                         const ecco::Model::PerVertexColors& perVertexColors,
                         const ecco::Model::TexCoords& uvCoords);


                void EnableInstancing(); //assert if m_hasInstancing is false
                void DisableInstancing();
                bool GetHasInstancing() const;
                bool GetIsInstancing() const;

                void EnablePerVertexColors(); //assert if m_hasPerVertexColors is false
                void DisablePerVertexColors();
                bool GetHasPerVertexColors() const;
                bool GetIsPerVertexColors() const;

            private:
                //Creates VAO, m_vboVerts, m_vboNormsl, m_eboTriangles
                void CreateVAOWithGeometry(std::shared_ptr<Model::Model> model);
                void CreateInstancingVBO(const ecco::Model::InstancePositions& instancePositions);
                void CreatePerVertexColorVBO(const ecco::Model::PerVertexColors& perVertexColors);
                void CreatePerVertexTexCoords(const ecco::Model::PerVertexColors& perVertexTexCoords);

                //Geometry information
                GLVAO m_vao;
                GLVBOVertex m_vboVerts;
                GLVBONormal m_vboNorms;
                GLEBOTriangle m_eboTriangles;


                int m_numVertices, m_numTriangles;

                bool m_hasInstancing;
                bool m_isInstancing;
                GLVBOInstancePosition m_vboInstancedPositions;

                bool m_hasPerVertexColors;
                bool m_isPerVertexColors;
                GLVBOPerVertexColor m_perVertexColors;

                bool m_hasTexCoords;
                bool m_isUsingTexCoords;
                GLVBOTexCoord m_vboTexCoords;

                //Model owns shared ptr to this object
                std::weak_ptr<ecco::Model::Model> m_model;
                std::weak_ptr<ecco::Material::Material> m_material; //Get material handle here


                //do this later... iirc you can't just call glBufferData every frame this will create new memory
                //you need to call glsubbufferdata or something to update when using dynamic vbo updates
                bool m_hasFrequentUpdates; //Set to true if this object is updated every frame~ this will be to set dynamic draw in gl

        };
    }
}

#endif
