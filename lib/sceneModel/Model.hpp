#ifndef MODEL_HPP
#define MODEL_HPP

#include "../ecco/EccoObject.hpp"
#include <glm/glm.hpp>

namespace ecco
{
    namespace Model
    {
        class Model : ecco::EccoObject
        {
            public:
                Model() = delete;
                Model(Model&) = delete;
                explicit Model(std::string name);
                ~Model();
                void LoadModel(std::string filename);
                void SetTexCoords(const std::vector<glm::vec2>& coords);
                //idk what should be in here figure it out.
                //Different loaders?
            private:
                int m_numVertices;
                int m_numTriangles;
                bool m_hasTexCoords;
        };
    }
}
#endif
