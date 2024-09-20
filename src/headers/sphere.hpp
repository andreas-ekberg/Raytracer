#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "../glmlib/glm.hpp"
#include "../glmlib/gtx/string_cast.hpp"
#include "ray.hpp"

class Sphere
{
public:
    Sphere();
    Sphere(const glm::dvec3 &center, float radius);

    bool hit(const Ray &r, float t_min, float t_max) const;

    glm::dvec3 center() const;
    float radius() const;

private:
    glm::dvec3 m_center;
    float m_radius;
};

#endif // SPHERE_H