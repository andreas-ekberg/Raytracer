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
    bool somethingHit = false;
    Material::MaterialType materialType;
    double smallestTLength = std::numeric_limits<double>::infinity();
    glm::dvec3 possibleIntersectionPoint(0, 0, 0);
    int indexCounter = 0;
    int objectIndex = 0;

    const auto &polygons = Polygon::polygons;
    for (auto &p : polygons)
    {
        glm::dvec3 intersectionPoint = p->isHit(*this);
        didntHit = glm::all(glm::isnan(intersectionPoint));
        if (!didntHit)
        {
            somethingHit = true;
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

        float randomNum = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));

        return randomNum >= (float)0.5 ? this->calculateRayPath(possibleIntersectionPoint) : this;
        //return this;
    }

    return this;
}

Ray *Ray::calculateRayPath(glm::dvec3 &hitPosition)
{
    // New direction from reflect
    glm::dvec3 newDirection(0, 0, 0);
    if (this->hitObjectMaterial == Material::Lambertian)
    {
        newDirection = getRandomDirection(this->rayHitNormal);
    }
    else
    {
        glm::dvec3 inDirection = this->rayDirection;
        newDirection = inDirection - 2.0 * ((glm::dot(inDirection, this->rayHitNormal)) * this->rayHitNormal);
    }

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
        if (randomNum >= 0.5f)
        {
            // std::cout << randomNum << " ";

            return newRay->calculateRayPath(possibleIntersectionPoint);
        }
        else
            return this;
    }

    return this;
}

glm::dvec3 Ray::getColorOfRayPath(Light &lightSource)
{
    if (this->nextRay == nullptr)
    {
        return this->calculateIrradiance(lightSource);
    }
    Ray *rayPointer = this->nextRay;
    // glm::dvec3 totColor = glm::dvec3(1, 1, 1);
    //  Loop to get to end of ray path

    glm::dvec3 totColor = glm::dvec3(0, 0, 0);

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

    double cosOmegaX = glm::clamp(glm::dot(glm::normalize(this->rayHitNormal), LightToPointDirection) / distance, 0.0, (double)INFINITY);
    // std::cout << cosOmegaX << "\n";

    double cosOmegaY = -1.0 * glm::dot(lightNormal, LightToPointDirection) / distance;

    if (cosOmegaY < 0.0)
    {
        cosOmegaY = 0.0;
    }

    double G = (cosOmegaX * cosOmegaY) / (distance * distance);

    double Le = lightSource.getWatt() / (M_PI);

    double E = isVisible * Le * G * lightArea;

    // Too bright / pi
    return (this->rayColor * E);
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

localDirection Ray::getRandomLocalDirection()
{
    localDirection result;

    double azimuth = (2.0 * M_PI) * ((double)rand()) / (((double)RAND_MAX));
    double inclination = ((double)rand()) / (((double)RAND_MAX));

    result.azimuth = azimuth;
    result.inclination = acos(sqrt(1.0 - inclination));

    return result;
}

glm::dvec3 Ray::getRandomDirection(glm::dvec3 normals)
{
    localDirection dir = getRandomLocalDirection();

    glm::dvec3 localDir = hemisphericalToCartesian(dir);
    glm::dvec3 worldDir = glm::normalize(localCartesianToWorldCartesian(localDir, normals));

    if (glm::dot(worldDir, normals) < 0.0f)
    {
        worldDir = -1.0 * worldDir;
    }

    return worldDir;
}
glm::dvec3 Ray::hemisphericalToCartesian(localDirection dir)
{
    return glm::dvec3(cos(dir.azimuth) * sin(dir.inclination), sin(dir.azimuth) * sin(dir.inclination), cos(dir.inclination));
}
glm::dvec3 Ray::localCartesianToWorldCartesian(glm::dvec3 localDir, glm::dvec3 normals)
{
    double x_0 = localDir.x;
    double y_0 = localDir.y;
    double z_0 = localDir.z;

    glm::dvec3 z_l = glm::normalize(normals);

    glm::dvec3 arbitraryVec = (glm::abs(z_l.x) > 0.99) ? glm::dvec3(0.0, 1.0, 0.0) : glm::dvec3(1.0, 0.0, 0.0);

    glm::dvec3 x_l = glm::normalize(glm::cross(arbitraryVec, z_l));
    glm::dvec3 y_l = glm::cross(z_l, x_l);

    glm::dvec3 res(x_0 * x_l.x + y_0 * y_l.x + z_0 * z_l.x,
                   x_0 * x_l.y + y_0 * y_l.y + z_0 * z_l.y,
                   x_0 * x_l.z + y_0 * y_l.z + z_0 * z_l.z);

    return glm::normalize(res);
}
#endif