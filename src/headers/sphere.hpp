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
    glm::dvec3 isHit(Ray ray) override;
    Material getPolygonMaterial() override;
    glm::dvec3 getNormal(Ray ray) override;

    glm::dvec3 sphereCenter;
    double sphereRadius;
};

#endif // SPHERE_H