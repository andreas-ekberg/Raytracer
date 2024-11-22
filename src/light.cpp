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
    normal = glm::normalize(glm::cross(p[2] - p[0], p[1] - p[0]));
    color = glm::dvec3(1, 1, 1);
    area = glm::distance(p[0], p[1]) * glm::distance(p[0], p[2]);
    watt = _watt;
}

glm::dvec3 Light::getNormal()
{
    return normal;
}

Material Light::getPolygonMaterial()
{
    return Material::Light;
}

glm::dvec3 Light::getColor()
{
    return color;
}

glm::dvec3 Light::isHit(Ray ray)
{
    glm::dvec3 v = p[2];
    glm::dvec3 s = ray.rayOrigin;
    glm::dvec3 direction = ray.rayDirection;
    double t = glm::dot((v - s), normal) / glm::dot(direction, normal);

    if (t < DBL_EPSILON)
    {
        return glm::dvec3(NAN, NAN, NAN);
    }

    glm::dvec3 xi = ray.position(t);
    glm::dvec3 c1 = p[3] - v;
    glm::dvec3 c2 = p[0] - v;

    double a = glm::dot((xi - v), c1) / glm::dot(c1, c1);
    double b = glm::dot((xi - v), c2) / glm::dot(c2, c2);
    if (a >= 0.0 && a <= 1.0 && b >= 0.0 && b <= 1.0)
    {
        return xi;
    }
    else
    {
        return glm::dvec3(NAN, NAN, NAN);
    }
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
    double randomX = ((double)rand()) / ((RAND_MAX + 1.0));
    double randomY = ((double)rand()) / ((RAND_MAX + 1.0));

    glm::dvec3 edge1 = p[1] - p[0];
    glm::dvec3 edge2 = p[2] - p[0];

    // Change from 4 to actuall z height //

    glm::dvec3 randomPoint = p[0] + randomX * edge1 + randomY * edge2;

    return randomPoint;
}

#endif
