#ifndef RAY_CPP
#define RAY_CPP
#define _USE_MATH_DEFINES

#include "headers/ray.hpp"
#include "headers/material.hpp"
#include "headers/polygon.hpp"
#include <iostream>
#include <cmath>

Ray::Ray()
{
    rayOrigin = glm::dvec3(0, 0, 0);
    rayDirection = glm::dvec3(0, 0, 0);
}

Ray::Ray(const glm::dvec3 &origin, const glm::dvec3 &direction) : rayOrigin(origin), rayDirection(direction)
{
    nextRay = nullptr;
    prevRay = nullptr;
}

const glm::dvec3 &Ray::origin() const { return rayOrigin; }
const glm::dvec3 &Ray::direction() const { return rayDirection; }

glm::dvec3 Ray::position(double t) const
{
    return rayOrigin + rayDirection * t;
}

Ray *Ray::calculateRayPath(std::vector<Polygon *> &polygons)
{
    bool didntHit = true;
    Material::MaterialType materialType;
    for (auto &p : polygons)
    {
        glm::dvec3 intersectionPoint = p->isHit(*this);
        didntHit = glm::all(glm::isnan(intersectionPoint));
        if (!didntHit)
        {
            materialType = p->getPolygonMaterial().materialType;
            rayHitNormal = p->getNormal();
            rayHitPoint = intersectionPoint;

            switch (materialType)
            {
            case Material::MaterialType::Mirror:
                this->rayColor = glm::dvec3(1, 1, 1);
                return this->calculateRayPath(intersectionPoint, polygons);
                break;
            case Material::MaterialType::Lambertian:
                this->rayColor = p->getColor();
                srand(time(0)); // Seed the random number generator
                float randomNum = 1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2 - 1)));
                return randomNum == (float)1 ? this->calculateRayPath(intersectionPoint, polygons) : this;
                // return this;
            }
        }
    }
    return this;
}

Ray *Ray::calculateRayPath(glm::dvec3 hitPosition, std::vector<Polygon *> &polygons)
{

    // New direction from reflec
    glm::dvec3 inDirection = rayDirection;
    glm::dvec3 newDirection = inDirection - 2.0 * ((glm::dot(inDirection, this->rayHitNormal)) * this->rayHitNormal);

    Ray *newRay = new Ray(hitPosition, newDirection);
    this->nextRay = newRay;
    newRay->prevRay = this;

    // Another ray if hit mirror  return newRay.calculateRayPath();
    Material::MaterialType materialType;
    bool didntHit = true;
    for (auto &p : polygons)
    {
        glm::dvec3 intersectionPoint = p->isHit(*newRay);
        didntHit = glm::all(glm::isnan(intersectionPoint));
        newRay->rayHitPoint = intersectionPoint;
        if (!didntHit)
        {
            // if hit
            materialType = p->getPolygonMaterial().materialType;
            newRay->rayHitNormal = p->getNormal();
            switch (materialType)
            {
            case Material::MaterialType::Mirror:
                // std::cout << "hit mirror";
                newRay->rayColor = glm::dvec3(1, 1, 1);

                return newRay->calculateRayPath(intersectionPoint, polygons);
                break;
            case Material::MaterialType::Lambertian:
                newRay->rayColor = p->getColor();
                float randomNum = (float)rand() / (float)RAND_MAX;
                return randomNum < (float)0.5f ? newRay->calculateRayPath(intersectionPoint, polygons) : this;
                break;
            }
        }
    }

    return this;
}
glm::dvec3 Ray::getColorOfRayPath(Light &lightSource, std::vector<Polygon *> &polygons)
{
    Ray *rayPointer = this;
    // glm::dvec3 totColor = glm::dvec3(1, 1, 1);
    //  Loop to get to end of ray path
    while (rayPointer->nextRay != nullptr)
    {
        rayPointer = rayPointer->nextRay;
    }
    // Check what hit

    glm::dvec3 totColor = rayPointer->calculateIrradiance(lightSource, polygons);
    if (rayPointer->prevRay != nullptr)
    {
        rayPointer = rayPointer->prevRay;
    }
    // Get color from end -> start
    while (rayPointer->prevRay != nullptr)
    {

        glm::dvec3 irradiance = rayPointer->calculateIrradiance(lightSource, polygons);
        totColor = irradiance + rayPointer->rayColor * totColor;
        rayPointer = rayPointer->prevRay;
    }
    glm::dvec3 irradiance = rayPointer->calculateIrradiance(lightSource, polygons);
    totColor = irradiance + rayPointer->rayColor * totColor;
    return totColor;
}

glm::dvec3 Ray::calculateIrradiance(Light &lightSource, std::vector<Polygon *> &polygons)
{

    // Random point på lampan
    glm::dvec3 LightToPointDirection = lightSource.getRandomPoint() - this->rayHitPoint;
    glm::dvec3 lightNormal = lightSource.getNormal();

    // std::cout << glm::to_string(LightToPointDirection) << std::endl;

    double lightArea = lightSource.getArea();
    // std::cout << lightArea << "\n";

    double distance = glm::distance(LightToPointDirection, this->origin());

    double cosOmegaX = glm::clamp(glm::dot(glm::normalize(this->rayHitNormal), LightToPointDirection) / distance, 0.0, (double)INFINITY);

    double cosOmegaY = glm::dot(-lightNormal, LightToPointDirection) / distance;

    if (cosOmegaY < 0.0)
    {
        cosOmegaX = 0.0;
    }
    double G = (cosOmegaX * cosOmegaY) / (distance * distance);

    double E = lightArea * G * lightSource.getWatt() / M_PI;

    // Too bright / pi
    return this->rayColor * E;
}
glm::dvec3 Ray::calculateOffsetRay(double pixelSizeX, double pixelSizeY)
{
    double randomNumberX = ((double)rand() / (double)RAND_MAX) - 1.0;
    double randomNumberY = ((double)rand() / (double)RAND_MAX) - 1.0;
    glm::dvec3 offset = glm::dvec3(0.0, randomNumberX * pixelSizeX, randomNumberY * pixelSizeY);
    glm::dvec3 newRayDirection = rayDirection + offset;
    newRayDirection = glm::normalize(newRayDirection);
    rayDirection = newRayDirection;
    return newRayDirection;
}

#endif