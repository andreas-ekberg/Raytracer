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
    double smallestTLength = (double)INFINITY;
    glm::dvec3 possibleIntersectionPoint(0, 0, 0);
    int indexCounter = 0;
    int objectIndex = 0;
    for (auto &p : polygons)
    {

        glm::dvec3 intersectionPoint = p->isHit(*this);
        didntHit = glm::all(glm::isnan(intersectionPoint));

        double currentTLength = glm::length(intersectionPoint - this->origin());

        if (smallestTLength > currentTLength)
        {
            smallestTLength = currentTLength;
            possibleIntersectionPoint = intersectionPoint;
            objectIndex = indexCounter;
        }
        indexCounter++;
    }
    // std::cout << objectIndex;
    if (objectIndex == 6)
    {
        // std::cout << "hej";
    }
    materialType = polygons[objectIndex]->getPolygonMaterial().materialType;
    hitObjectMaterial = materialType;

    rayHitNormal = polygons[objectIndex]->getNormal();
    rayHitPoint = possibleIntersectionPoint;

    switch (materialType)
    {
    case Material::MaterialType::Mirror:
        this->rayColor = glm::dvec3(1, 1, 1);
        return this->calculateRayPath(possibleIntersectionPoint, polygons);
        break;
    case Material::MaterialType::Lambertian:
        this->rayColor = polygons[objectIndex]->getColor();
        // srand(time(0)); // Seed the random number generator
        float randomNum = 1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2 - 1)));
        return randomNum == (float)1 ? this->calculateRayPath(possibleIntersectionPoint, polygons) : this;
    }
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
    double smallestTLength = (double)INFINITY;
    glm::dvec3 possibleIntersectionPoint(0, 0, 0);
    int indexCounter = 0;
    int objectIndex = 0;
    for (auto &p : polygons)
    {

        glm::dvec3 intersectionPoint = p->isHit(*newRay);
        didntHit = glm::all(glm::isnan(intersectionPoint));

        double currentTLength = glm::length(intersectionPoint - newRay->origin());

        if (smallestTLength > currentTLength)
        {
            smallestTLength = currentTLength;
            possibleIntersectionPoint = intersectionPoint;
            objectIndex = indexCounter;
        }

        indexCounter++;
    }

    newRay->rayHitPoint = possibleIntersectionPoint;

    // if hit
    materialType = polygons[objectIndex]->getPolygonMaterial().materialType;
    newRay->hitObjectMaterial = materialType;
    newRay->rayHitNormal = polygons[objectIndex]->getNormal();
    switch (materialType)
    {
    case Material::MaterialType::Mirror:
        // std::cout << "hit mirror";
        newRay->rayColor = glm::dvec3(1, 1, 1);
        return newRay->calculateRayPath(possibleIntersectionPoint, polygons);
        break;
    case Material::MaterialType::Lambertian:
        newRay->rayColor = polygons[objectIndex]->getColor();
        float randomNum = (float)rand() / (float)RAND_MAX;
        return randomNum < (float)0.5f ? newRay->calculateRayPath(possibleIntersectionPoint, polygons) : this;
        break;
    }
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

    glm::dvec3 totColor(0, 0, 0);
    switch (rayPointer->hitObjectMaterial)
    {
    case Material::MaterialType::Mirror:
        break;
    case Material::MaterialType::Lambertian:
        totColor = rayPointer->calculateIrradiance(lightSource, polygons);
        break;
    }

    // Get color from end -> start
    while (rayPointer->prevRay != nullptr)
    {
        // std::cout << " here ";
        rayPointer = rayPointer->prevRay;
        glm::dvec3 irradiance(0, 0, 0);
        switch (rayPointer->hitObjectMaterial)
        {
        case Material::MaterialType::Mirror:
            break;
        case Material::MaterialType::Lambertian:
            irradiance = rayPointer->calculateIrradiance(lightSource, polygons);
            break;
        }
        totColor = irradiance + rayPointer->nextRay->rayColor * totColor;
    }

    return totColor;
}

glm::dvec3 Ray::calculateIrradiance(Light &lightSource, std::vector<Polygon *> &polygons)
{

    // Random point på lampan
    glm::dvec3 randomPoint = lightSource.getRandomPoint();
    glm::dvec3 LightToPointDirection = randomPoint - this->rayHitPoint;
    glm::dvec3 lightNormal = lightSource.getNormal();
    double isVisible = this->isVisible(this->rayHitPoint, randomPoint, lightSource, polygons);

    double lightArea = lightSource.getArea();

    double distance = glm::length(LightToPointDirection);

    // std::cout << glm::to_string(lightSource.getRandomPoint()) << std::endl;

    double cosOmegaX = glm::clamp(glm::dot(glm::normalize(this->rayHitNormal), glm::normalize(LightToPointDirection)), 0.0, (double)INFINITY);
    // std::cout << cosOmegaX << "\n";

    double cosOmegaY = -1.0 * glm::dot(lightNormal, glm::normalize(LightToPointDirection));

    if (cosOmegaY < 0.0)
    {
        cosOmegaX = 0.0;
    }

    double G = (cosOmegaX * cosOmegaY) / (distance * distance);

    double E = lightArea * G * lightSource.getWatt() / M_PI;

    // Too bright / pi
    return this->rayColor * E * isVisible;
}

int Ray::isVisible(glm::dvec3 intersectionPoint, glm::dvec3 randomPointOnLight, Light &lightSource, std::vector<Polygon *> &polygons)
{

    double smallestTLength = (double)INFINITY;
    int indexCounter = 0;
    int objectIndex = 0;

    Ray rayCast = Ray(randomPointOnLight, glm::normalize(intersectionPoint - randomPointOnLight));
    for (auto &p : polygons)
    {

        glm::dvec3 intersectionPoint = p->isHit(rayCast);

        double currentTLength = glm::length(intersectionPoint - rayCast.origin());

        if (smallestTLength > currentTLength)
        {
            smallestTLength = currentTLength;
            objectIndex = indexCounter;
        }

        indexCounter++;
    }

    double lengthBetweenLightAndObject = glm::length(intersectionPoint - randomPointOnLight);
    if (lengthBetweenLightAndObject != smallestTLength)
    {
        return 0;
    }
    else
    {
        return 1;
    }
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