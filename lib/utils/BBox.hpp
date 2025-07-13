#pragma once

#include <glm/glm.hpp>



//Define this and make classes like box, sphere, cone etc... inherit from it
class BasicGeometry;

//forward declare


namespace ecco {
    //oh hmm when forward declaring you have to define the namespace entirely you can't do Model::Model
    namespace Model {
        class Model;
        class SubMesh;
    }

    class BBox {
        public:
            BBox();
            BBox(glm::dvec3 min, glm::dvec3 max);
            BBox(const Model::Model& model);
            BBox(const Model::SubMesh& model);
            BBox(const BasicGeometry& geom);

            glm::dvec3 GetSpanVector() const;
            double GetSpan() const;
            double GetVolume() const;

        private:
            glm::dvec3 m_min;
            glm::dvec3 m_max;

    };
}
