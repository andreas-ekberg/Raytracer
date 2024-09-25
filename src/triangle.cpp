#ifndef TRIANGLE_CPP
#define TRIANGLE_CPP

#include "headers/triangle.hpp"

Triangle::Triangle()
{
}

Triangle::Triangle(glm::dvec3 _p1, glm::dvec3 _p2, glm::dvec3 _p3, glm::dvec3 _color, Material materialType)
{
    p[0] = _p1;
    p[1] = _p2;
    p[2] = _p3;
    material.setMaterialColor(_color);
    color = _color;
    normal = glm::normalize(glm::cross(p[1] - p[0], p[2] - p[0]));
    material = materialType;
}

glm::dvec3 Triangle::getColor()
{
    return color;
}

glm::dvec3 Triangle::isHit(Ray ray)
{
    glm::dvec3 v0 = p[0];
    glm::dvec3 v1 = p[1];
    glm::dvec3 v2 = p[2];
    glm::dvec3 ps = ray.rayOrigin;

    glm::dvec3 e1 = v1 - v0;
    glm::dvec3 e2 = v2 - v0;
    glm::dvec3 T = ps - v0;
    glm::dvec3 direction = glm::normalize(ray.rayDirection);

    glm::dvec3 P = glm::cross(direction, e2);
    glm::dvec3 Q = glm::cross(T, e1);

    glm::dvec3 tuv = (1 / glm::dot(P, e1)) * glm::dvec3(glm::dot(Q, e2), glm::dot(P, T), glm::dot(Q, direction));
    double t = tuv.x;
    if (t < DBL_EPSILON || tuv.y <= 0.0 || tuv.z <= 0.0 || tuv.y + tuv.z >= 1.0)
    {
        return glm::dvec3(NAN, NAN, NAN);
    }
    glm::dvec3 intersectionPoint = getPositionBarycentric(tuv.y, tuv.z);
    return intersectionPoint;
}

glm::dvec3 Triangle::getPositionBarycentric(double u, double v)
{
    return (1 - u - v) * p[0] + u * p[1] + v * p[2];
}

Material Triangle::getPolygonMaterial()
{
    return material;
}

glm::dvec3 Triangle::getNormal()
{
    return normal;
}
#endif
