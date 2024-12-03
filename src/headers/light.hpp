#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../glmlib/glm.hpp"
#include "../glmlib/gtx/string_cast.hpp"
#include "polygon.hpp"
#include "ray.hpp"

#include <iostream>

class Light : public Polygon
{
public:
    // Constructor
    Light();
    Light(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 bottomLeft, glm::dvec3 bottomRight, double watt);

    glm::dvec3 getNormal(const Ray &ray) const override;
    Material getPolygonMaterial() override;
    glm::dvec3 getColor() override;
    glm::dvec3 isHit(const Ray &ray)const  override;

    glm::dvec3 getRandomPoint() const;
    double getArea() const;
    double  getWatt() const;

private:
    glm::dvec3 p[4];
    double area;
    double watt;
    glm::dvec3 color;
};

#endif