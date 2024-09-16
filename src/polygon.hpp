#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "glmlib/glm.hpp"
#include "glmlib/gtx/string_cast.hpp"
#include "ray.hpp"

class Polygon {
public:
    // Constructor
    Polygon() {}

    // Destructor
     ~Polygon() {}

    virtual glm::dvec3 isHit(Ray ray) = 0;

    virtual glm::dvec3 getColor() = 0;

private:
    

protected:
    glm::dvec3 normal;
};

#endif