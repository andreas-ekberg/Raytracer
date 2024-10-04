#ifndef RAY_HPP
#define RAY_HPP

#include <ppl.h>
#include "../glmlib/glm.hpp"
#include "../glmlib/gtx/string_cast.hpp"
#include "headers/light.hpp"
#include "headers/material.hpp"

// Forward declaration polygon class
class Polygon;

class Ray
{
public:
    Ray();
    Ray(const glm::dvec3 &origin, const glm::dvec3 &direction);
    const glm::dvec3 &origin() const;
    const glm::dvec3 &direction() const;

    glm::dvec3 position(double t) const;
    glm::dvec3 calculateOffsetRay(double pixelSizeX, double pixelSizeY);

    // Second hits
    Ray *calculateRayPath(glm::dvec3 hitPosition, std::vector<Polygon *> &polygons);
    // First ray
    Ray *calculateRayPath(std::vector<Polygon *> &polygons);

    glm::dvec3 getColorOfRayPath(Light &lightSource, std::vector<Polygon *> &polygons);
    glm::dvec3 calculateIrradiance(Light &lightSource, std::vector<Polygon *> &polygons);

    int isVisible(glm::dvec3 intersectionPoint, glm::dvec3 randomPointOnLight, Light &lightSource, std::vector<Polygon *> &polygons);

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