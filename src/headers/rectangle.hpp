#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "polygon.hpp"
#include "ray.hpp"
#include "../glmlib/glm.hpp"
#include "../glmlib/gtx/string_cast.hpp"

class Rectangle : public Polygon
{
public:
    // Constructor
    Rectangle();
    Rectangle(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 bottomLeft, glm::dvec3 bottomRight, glm::dvec3 _color, Material materialType);

    // Destructor
    ~Rectangle();

    glm::dvec3 isHit(const Ray &ray) const override;

    glm::dvec3 getColor() override;

    Material getPolygonMaterial() override;

    glm::dvec3 getNormal(const Ray &ray) const override;

private:
    glm::dvec3 p[4];
    glm::dvec3 color;
};

#endif