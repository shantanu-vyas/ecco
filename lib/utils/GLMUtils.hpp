#ifndef GLMUTILS_HPP
#define GLMUTILS_HPP

#include <glm/glm.hpp>

namespace glm {

    static bool dvec3Sort(const glm::dvec3 &a, const glm::dvec3 &b) {
        if (a.x != b.x)
            return a.x < b.x;
        if (a.y != b.y)
            return a.y < b.y;
        return a.z < b.z;
    };
}

//not a glm util but im lazy

#endif
