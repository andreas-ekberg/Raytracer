#ifndef RECTANGLE_CPP
#define RECTANGLE_CPP

#include "headers/rectangle.hpp"

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(glm::dvec3 topRight, glm::dvec3 topLeft, glm::dvec3 bottomRight, glm::dvec3 bottomLeft, glm::dvec3 _color, Material materialType)
{
    p[0] = topRight;
    p[1] = topLeft;
    p[2] = bottomRight;
    p[3] = bottomLeft;
    color = _color;
    material.setMaterialColor(_color);
    normal = glm::normalize(glm::cross(p[3] - p[2], p[0] - p[2]));
    material = materialType;
}

Rectangle::~Rectangle() {}

glm::dvec3 Rectangle::isHit(const Ray &ray) const
{
    glm::dvec3 v = p[2];
    glm::dvec3 s = ray.rayOrigin;
    glm::dvec3 direction = ray.rayDirection;
    double t = glm::dot((v - s), normal) / glm::dot(direction, normal);

    if (t < 1e-3)
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

glm::dvec3 Rectangle::getColor()
{
    return color;
}

Material Rectangle::getPolygonMaterial()
{
    return material;
}

glm::dvec3 Rectangle::getNormal(const Ray &ray) const
{
    return normal;
}

#endif
