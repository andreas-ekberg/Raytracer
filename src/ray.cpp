#ifndef RAY_CPP
#define RAY_CPP

#include "headers/ray.hpp"
#include "headers/material.hpp"
#include "headers/polygon.hpp"
#include <iostream>

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

Ray *Ray::calculateRayPath(std::vector<Polygon *> polygons)
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

            switch (materialType)
            {
            case Material::MaterialType::Mirror:
                return this->calculateRayPath(p->getNormal(), intersectionPoint, polygons);
                break;
            case Material::MaterialType::Lambertian:
                this->rayColor = p->getColor();
                srand(time(0)); // Seed the random number generator
                float randomNum = 1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2 - 1)));
                return randomNum == (float)1 ? this->calculateRayPath(p->getNormal(), intersectionPoint, polygons) : this;
                return this;
            }
        }
    }
    return this;
}

Ray *Ray::calculateRayPath(glm::dvec3 normal, glm::dvec3 hitPosition, std::vector<Polygon *> polygons)
{

    // New direction from reflec
    glm::dvec3 inDirection = rayDirection;
    glm::dvec3 newDirection = inDirection - 2.0 * ((glm::dot(inDirection, normal)) * normal);

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
        if (!didntHit)
        {
            // if hit
            materialType = p->getPolygonMaterial().materialType;
            switch (materialType)
            {
            case Material::MaterialType::Mirror:
                // std::cout << "hit mirror";
                newRay->rayColor = glm::dvec3(1, 1, 1);
                return newRay->calculateRayPath(p->getNormal(), intersectionPoint, polygons);
                break;
            case Material::MaterialType::Lambertian:
                newRay->rayColor = p->getColor();
                srand(time(0)); // Seed the random number generator
                float randomNum = 1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2 - 1)));
                return randomNum == (float)1 ? newRay->calculateRayPath(p->getNormal(), intersectionPoint, polygons) : this;
                break;
            }
        }
    }

    return this;
}
glm::dvec3 Ray::getColorOfRayPath()
{
    Ray *rayPointer = this;
    glm::dvec3 totColor = glm::dvec3(0, 0, 0);
    while (rayPointer->nextRay != nullptr)
    {
        rayPointer = rayPointer->nextRay;
    }

    return rayPointer->rayColor;
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