#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../glmlib/glm.hpp"
#include "../glmlib/gtx/string_cast.hpp"
#include <iostream>

class Light
{
public:
    // Constructor
    Light();
    Light(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 bottomLeft, glm::dvec3 bottomRight, double watt);

    glm::dvec3 getNormal();
    glm::dvec3 getRandomPoint();
    double getArea();
    double getWatt();

private:
    glm::dvec3 p[4];
    glm::dvec3 normal;
    double area;
    double watt;

    glm::dvec3 color;
};

#endif