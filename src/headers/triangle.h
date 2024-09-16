#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../polygon.hpp"
#include "../ray.hpp"
#include "../glmlib/glm.hpp"
#include "../glmlib/gtx/string_cast.hpp"

class Triangle : public Polygon
{

public:
    Triangle();
    Triangle(glm::dvec3 _p1, glm::dvec3 _p2, glm::dvec3 _p3, glm::dvec3 _color);

    glm::dvec3 getColor() override;

    glm::dvec3 isHit(Ray ray) override;
    glm::dvec3 getPositionBarycentric(double u, double v);



private:
    glm::dvec3 p[3];
    glm::dvec3 color;

};

#endif