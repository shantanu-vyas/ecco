#ifndef VAOMODEL_HPP
#define VAOMODEL_HPP

#include <GL/gl.h>
#include "../ecco/EccoObject.hpp";
#include "../sceneModel/Model.hpp"
#include <memory>

namespace ecco
{
    namespace OpenGL
    {
        class VAOModel : ecco::EccoObject
        {
            public:
                explicit VAOModel(const std::shared_ptr<Model::Model> model);
                VAOModel() = delete;
                VAOModel(VAOModel&) = delete;
                ~VAOModel();

                void RecreateVAO(std::shared_ptr<Model::Model> model);
                void DeleteVao();

            private:
                GLuint m_vao, m_vboVerts, m_vboNorms, m_vboTexCoords;

        };
    }
}

#endif
