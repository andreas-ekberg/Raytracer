#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "../glmlib/glm.hpp"
#include "../glmlib/gtx/string_cast.hpp"
#include "ray.hpp"
#include "polygon.hpp"

class Sphere : public Polygon
{
public:
    Sphere();
    Sphere(glm::dvec3 center, double radius, glm::dvec3 _color, Material materialType);
    ~Sphere();

    glm::dvec3 getColor() override;
    glm::dvec3 isHit(const Ray &ray) const override;
    Material getPolygonMaterial() override;
    glm::dvec3 getNormal(const Ray &ray) const override;

    glm::dvec3 sphereCenter;
    glm::dvec3 color;
    double sphereRadius;
};

#endif // SPHERE_H