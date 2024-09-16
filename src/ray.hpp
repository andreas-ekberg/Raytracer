#ifndef RAY_HPP
#define RAY_HPP

#include "glmlib/glm.hpp"
#include "glmlib/gtx/string_cast.hpp"

class Ray
{
public:
    Ray()
    {
        rayOrigin = glm::dvec3(0, 0, 0);
        rayDirection = glm::dvec3(0, 0, 0);
    }
    Ray(const glm::dvec3 &origin, const glm::dvec3 &direction) : rayOrigin(origin), rayDirection(direction) {}

    const glm::dvec3 &origin() const { return rayOrigin; }
    const glm::dvec3 &direction() const { return rayDirection; }

    glm::dvec3 position(double t) const
    {
        return rayOrigin + rayDirection * t;
    }

    glm::dvec3 rayOrigin;
    glm::dvec3 rayDirection;

    glm::dvec3 calculateOffsetRay(double pixelSizeX, double pixelSizeY) {
        double randomNumberX = ((double)rand() / (double)RAND_MAX) - 1.0;
        double randomNumberY = ((double)rand() / (double)RAND_MAX) - 1.0;
        glm::dvec3 offset = glm::dvec3(0.0, randomNumberX * pixelSizeX, randomNumberY * pixelSizeY);
        glm::dvec3 newRayDirection = rayDirection + offset;
        newRayDirection = glm::normalize(newRayDirection);
        rayDirection = newRayDirection;
        return newRayDirection;
    }
};

#endif