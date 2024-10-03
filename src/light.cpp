#ifndef LIGHT_CPP
#define LIGHT_CPP

#include "headers/light.hpp"

Light::Light()
{
}

Light::Light(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 bottomLeft, glm::dvec3 bottomRight, double _watt)
{
    p[0] = topLeft;
    p[1] = topRight;
    p[2] = bottomLeft;
    p[3] = bottomRight;
    normal = -glm::normalize(glm::cross(p[3] - p[2], p[0] - p[2]));
    color = glm::dvec3(1, 1, 1);
    area = glm::distance(p[0], p[1]) * glm::distance(p[2], p[3]);
    watt = _watt;
}

glm::dvec3 Light::getNormal()
{
    return this->normal;
}

double Light::getArea()
{
    return this->area;
}

double Light::getWatt()
{
    return this->watt;
}

glm::dvec3 Light::getRandomPoint()
{
    double randomX = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    double randomY = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);

    randomX = randomX * glm::distance(p[0], p[1]);
    randomY = randomY * glm::distance(p[2], p[3]);

    glm::dvec3 edge1 = p[3] - p[2];
    glm::dvec3 edge2 = p[0] - p[2];

    // Change from 4 to actuall z height //

    glm::dvec3 randomPoint = p[2] + randomX * edge1 + randomY * edge2;

    return randomPoint;
}

#endif
