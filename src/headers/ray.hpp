#ifndef RAY_HPP
#define RAY_HPP

#include <ppl.h>
#include "../glmlib/glm.hpp"
#include "../glmlib/gtx/string_cast.hpp"
#include "headers/material.hpp"

class Light;

struct localDirection
{
public:
    double azimuth = 0.0;
    double inclination = 0.0;
};

class Ray
{
public:
    Ray();
    Ray(const glm::dvec3 &origin, const glm::dvec3 &direction);

    Ray::~Ray();

    const glm::dvec3 &origin() const;
    const glm::dvec3 &direction() const;

    glm::dvec3 position(double t) const;
    glm::dvec3 calculateOffsetRay(double pixelSizeX, double pixelSizeY);

    // Second hits
    Ray *calculateRayPath(glm::dvec3 &hitPosition);
    // First ray
    Ray *calculateRayPath();

    glm::dvec3 getColorOfRayPath(Light &lightSource);
    glm::dvec3 calculateIrradiance(Light &lightSource);

    // Functions for random ray direction
    localDirection getRandomLocalDirection();
    glm::dvec3 getRandomDirection(glm::dvec3 normals);
    glm::dvec3 hemisphericalToCartesian(localDirection dir);
    glm::dvec3 localCartesianToWorldCartesian(glm::dvec3 localDir, glm::dvec3 normals);

    int isVisible(glm::dvec3 &intersectionPoint, glm::dvec3 &randomPointOnLight, Light &lightSource);

    // Ray stugg
    glm::dvec3 rayOrigin;
    glm::dvec3 rayDirection;
    glm::dvec3 rayColor;

    // Ray hit info
    glm::dvec3 rayHitNormal;
    glm::dvec3 rayHitPoint;
    Material::MaterialType hitObjectMaterial;

    // Ray path
    Ray *nextRay;
    Ray *prevRay;
};

#endif