#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "../glmlib/glm.hpp"
#include "../glmlib/gtx/string_cast.hpp"
#include "material.hpp"
#include "ray.hpp"

class Polygon
{
public:
    virtual glm::dvec3 isHit(const Ray &ray)const = 0;

    virtual glm::dvec3 getColor() = 0;

    virtual Material getPolygonMaterial() = 0;
    virtual glm::dvec3 getNormal(const Ray &ray) const = 0;

    static std::vector<Polygon *> polygons;

private:
protected:
    glm::dvec3 normal;
    Material material;
};

#endif