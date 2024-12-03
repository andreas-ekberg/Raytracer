#ifndef SPHERE_CPP
#define SPHERE_CPP
#include "headers/sphere.hpp"
#include <iostream>

Sphere::Sphere()
{
}

Sphere::Sphere(glm::dvec3 center, double radius, glm::dvec3 _color, Material materialType)
{
    color = _color;
    material.setMaterialColor(_color);
    material = materialType;
    sphereCenter = center;
    sphereRadius = radius;
}

Sphere::~Sphere() {}

glm::dvec3 Sphere::isHit(Ray &ray){
    glm::dvec3 rayNormalized = glm::normalize(ray.rayDirection);

    double c1 = glm::dot(rayNormalized, rayNormalized);
    double c2 = 2.0 * glm::dot(rayNormalized, ray.rayOrigin - sphereCenter);
    double c3 = glm::length(ray.rayOrigin - sphereCenter) * glm::length(ray.rayOrigin - sphereCenter) - sphereRadius * sphereRadius;

    double discriminant = c2 * c2 - 4.0 * c1 * c3;

    if(discriminant < 0.0){ // No intersection
        return glm::dvec3(NAN, NAN, NAN);
    }
    if(discriminant < 0.000001){ // One intersection | ray hit the edge of the sphere
        return ray.rayOrigin + rayNormalized * (-c2 + glm::sqrt(discriminant)) / (2.0 * c1); // rayOrigin + direction * t
        // + and - give the same t since only one intersection
    }
    if(discriminant > 0.0){ // Two intersections | ray hit the sphere
        double precision = 0.0000001;

        double t1 = (-c2 + glm::sqrt(discriminant)) / (2.0 * c1);
        double t2 = (-c2 - glm::sqrt(discriminant)) / (2.0 * c1);

        if(t1 < precision){
            t1 = -1;
        }
        if(t2 < precision){
            t2 = -1;
        }

        if(t1 < precision && t2 < precision){
            return glm::dvec3(NAN, NAN, NAN);
        }

        if(t1 < precision && t2 > precision){
            return ray.rayOrigin + rayNormalized * t2; // If t1 is negative, t2 is positive
        }
        if(t2 < precision && t1 > precision){
            return ray.rayOrigin + rayNormalized * t1; // If t2 is negative, t1 is positive
        }

        if(t1 < t2){
            return ray.rayOrigin + rayNormalized * t1; // t1 is closer
        }else{
            return ray.rayOrigin + rayNormalized * t2; // t2 is closer
        }
    }

    std::cout << "ERROR: Sphere isHit() should never reach this point" << std::endl;
    return glm::dvec3(NAN, NAN, NAN); // Should never reach this point

}

glm::dvec3 Sphere::getColor()
{
    return color;
}

Material Sphere::getPolygonMaterial()
{
    return material;
}

glm::dvec3 Sphere::getNormal(Ray &ray)
{
    glm::dvec3 intersectionPoint = this->isHit(ray);

    if(!glm::all(glm::isnan(intersectionPoint))){
        glm::dvec3 normal = glm::normalize(intersectionPoint - sphereCenter);
        return normal;
    }
    else{
        return glm::dvec3(NAN, NAN, NAN);
    }

}





#endif
