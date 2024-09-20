#ifndef RAY_HPP
#define RAY_HPP

#include <ppl.h>
#include "../glmlib/glm.hpp"
#include "../glmlib/gtx/string_cast.hpp"

class Ray
{
public:
    Ray();
    Ray(const glm::dvec3 &origin, const glm::dvec3 &direction);
    const glm::dvec3 &origin() const;
    const glm::dvec3 &direction() const;

    glm::dvec3 position(double t) const;
    glm::dvec3 calculateOffsetRay(double pixelSizeX, double pixelSizeY);
    //Ray calculateRayPath(glm::dvec3 normal, glm::dvec3 hitPosition, std::vector<std::unique_ptr<Polygon>> &polygons);

    glm::dvec3 rayOrigin;
    glm::dvec3 rayDirection;

    Ray *nextRay;
    Ray *prevRay;
};

#endif