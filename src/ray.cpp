#ifndef RAY_CPP
#define RAY_CPP
#define _USE_MATH_DEFINES

#include "headers/ray.hpp"
#include "headers/material.hpp"
#include "headers/polygon.hpp"
#include "headers/light.hpp"
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

Ray::~Ray()
{
    if (nextRay != nullptr)
    {
        delete nextRay;
        nextRay = nullptr;
    }
}

const glm::dvec3 &Ray::origin() const { return rayOrigin; }
const glm::dvec3 &Ray::direction() const { return rayDirection; }

glm::dvec3 Ray::position(double t) const
{
    return rayOrigin + rayDirection * t;
}

Ray *Ray::calculateRayPath()
{
    bool didntHit = true;
    Material::MaterialType materialType;
    double smallestTLength = std::numeric_limits<double>::infinity();
    glm::dvec3 possibleIntersectionPoint(0, 0, 0);
    int indexCounter = 0;
    int objectIndex = 0;

    const auto &polygons = Polygon::polygons;
    for (auto &p : polygons)
    {
        glm::dvec3 intersectionPoint = p->isHit(*this);
        if (!glm::all(glm::isnan(intersectionPoint)))
        {
            double currentTLength = glm::length(intersectionPoint - this->origin());
            if (currentTLength < smallestTLength)
            {
                smallestTLength = currentTLength;
                possibleIntersectionPoint = intersectionPoint;
                objectIndex = indexCounter;
            }
        }
        indexCounter++;
    }

    materialType = polygons[objectIndex]->getPolygonMaterial().materialType;
    hitObjectMaterial = materialType;

    rayHitNormal = polygons[objectIndex]->getNormal();
    rayHitPoint = possibleIntersectionPoint;

    switch (materialType)
    {
    case Material::MaterialType::Mirror:
        this->rayColor = glm::dvec3(1, 1, 1);
        return this->calculateRayPath(possibleIntersectionPoint);
        break;
    case Material::MaterialType::Light:
        this->rayColor = glm::dvec3(1, 1, 1);
        break;
    case Material::MaterialType::Lambertian:
        this->rayColor = Polygon::polygons[objectIndex]->getColor();
        // srand(time(0)); // Seed the random number generator
        float randomNum = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2 - 1)));

        return randomNum >= (float)0.5 ? this->calculateRayPath(possibleIntersectionPoint) : this;
    }

    return this;
}

Ray *Ray::calculateRayPath(glm::dvec3 &hitPosition)
{
    // New direction from reflec
    glm::dvec3 inDirection = this->rayDirection;
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
    for (auto &p : Polygon::polygons)
    {

        glm::dvec3 intersectionPoint = p->isHit(*newRay);
        didntHit = glm::all(glm::isnan(intersectionPoint));

        double currentTLength = glm::length(intersectionPoint - newRay->origin());

        if (currentTLength < smallestTLength)
        {
            smallestTLength = currentTLength;
            possibleIntersectionPoint = intersectionPoint;
            objectIndex = indexCounter;
        }

        indexCounter++;
    }

    newRay->rayHitPoint = possibleIntersectionPoint;

    // if hit
    materialType = Polygon::polygons[objectIndex]->getPolygonMaterial().materialType;

    newRay->hitObjectMaterial = materialType;
    newRay->rayHitNormal = Polygon::polygons[objectIndex]->getNormal();
    switch (materialType)
    {
    case Material::MaterialType::Mirror:
        // std::cout << "hit mirror";
        newRay->rayColor = glm::dvec3(1, 1, 1);
        return newRay->calculateRayPath(possibleIntersectionPoint);
        break;
    case Material::MaterialType::Light:
        newRay->rayColor = glm::dvec3(1, 1, 1);
        break;
    case Material::MaterialType::Lambertian:
        newRay->rayColor = Polygon::polygons[objectIndex]->getColor();
        float randomNum = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2 - 1)));
        if (randomNum >= 0.2f)
        {
            std::cout << randomNum << " ";

            return newRay->calculateRayPath(possibleIntersectionPoint);
        }
        else
            return this;
    }

    return this;
}

glm::dvec3 Ray::getColorOfRayPath(Light &lightSource)
{
    Ray *rayPointer = this;
    int rayHist = 1;
    // glm::dvec3 totColor = glm::dvec3(1, 1, 1);
    //  Loop to get to end of ray path
    while (rayPointer->nextRay != nullptr)
    {

        rayPointer = rayPointer->nextRay;
        rayHist++;
    }
    std::cout << rayHist;

    glm::dvec3 totColor = glm::dvec3(1, 0, 0);
    /*  switch (rayPointer->hitObjectMaterial)
     {
     case Material::MaterialType::Mirror:
         break;
     case Material::MaterialType::Lambertian:
         totColor = rayPointer->calculateIrradiance(lightSource);
         break;
     case Material::MaterialType::Light:
         totColor = glm::dvec3(1, 1, 1);
         break;
     } */
    // Get color from end -> start
    while (rayPointer->prevRay != nullptr)
    {
        glm::dvec3 irradiance(0, 0, 0);
        switch (rayPointer->hitObjectMaterial)
        {
        case Material::MaterialType::Mirror:
            // totColor = totColor;
            break;
        case Material::MaterialType::Lambertian:
            irradiance = rayPointer->calculateIrradiance(lightSource);
            totColor = irradiance + rayPointer->rayColor * totColor;
            // std::cout << irradiance.x << " " << irradiance.y << " " << irradiance.z << " \n";

            break;
        case Material::MaterialType::Light:
            totColor = glm::dvec3(1, 1, 1);
            break;
        }
        rayPointer = rayPointer->prevRay;
        // std::cout << totColor.x << " " << totColor.y << " " << totColor.z << " I WHIEL---------------------------------------------- \n";
    }
    // std::cout << totColor.x << " " << totColor.y << " " << totColor.z << " \n";

    return totColor;
}

glm::dvec3 Ray::calculateIrradiance(Light &lightSource)
{

    // Random point på lampan
    glm::dvec3 randomPoint = lightSource.getRandomPoint();
    glm::dvec3 LightToPointDirection = randomPoint - this->rayHitPoint;
    glm::dvec3 lightNormal = lightSource.getNormal();
    double isVisible = this->isVisible(this->rayHitPoint, randomPoint, lightSource);

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

    double Le = lightSource.getWatt() / (lightArea * M_PI);

    double E = isVisible * Le * G;

    // Too bright / pi
    return this->rayColor * E;
}

int Ray::isVisible(glm::dvec3 &intersectionPoint, glm::dvec3 &randomPointOnLight, Light &lightSource)
{

    double smallestTLength = (double)INFINITY;
    int indexCounter = 0;
    int objectIndex = 0;

    Ray rayCast = Ray(randomPointOnLight, glm::normalize(intersectionPoint - randomPointOnLight));
    for (auto &p : Polygon::polygons)
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