#ifndef TRIANGLE_CPP
#define TRIANGLE_CPP

#include "triangle/triangle.h"

Triangle::Triangle(){

}

Triangle::Triangle(glm::dvec3 _p1, glm::dvec3 _p2, glm::dvec3 _p3, glm::dvec3 _color)
{
    p[0] = _p1;
    p[1] = _p2;
    p[2] = _p3;
    color = _color;
}

glm::dvec3 Triangle::getColor() {
    return color;
}

glm::dvec3 Triangle::isHit(Ray ray){
    return glm::dvec3(NAN, NAN, NAN);
}

#endif

