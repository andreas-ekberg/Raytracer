#ifndef SPHERE_CPP
#define SPHERE_CPP
#include "headers/sphere.hpp"

Sphere::Sphere()
{
}

Sphere::Sphere(glm::dvec3 center, double radius, glm::dvec3 _color, Material materialType)
{
    material.setMaterialColor(_color);
    material = materialType;
    sphereCenter = center;
    sphereRadius = radius;
}

Sphere::~Sphere() {}

glm::dvec3 Sphere::isHit(Ray ray){

    double c1 = glm::dot(ray.rayDirection, ray.rayDirection);
    double c2 = 2.0 * glm::dot(ray.rayDirection, ray.rayOrigin - sphereCenter);
    double c3 = glm::length(ray.rayOrigin - sphereCenter) * glm::length(ray.rayOrigin - sphereCenter) - sphereRadius * sphereRadius;

    double discriminant = c2 * c2 - 4.0 * c1 * c3;

    if(discriminant < 0.0){ // No intersection
        return glm::dvec3(NAN, NAN, NAN);
    }
    if(discriminant == 0.0){ // One intersection | ray hit the edge of the sphere
        return ray.rayOrigin + ray.rayDirection * (-c2 + glm::sqrt(discriminant)) / (2.0 * c1); // rayOrigin + direction * t
        // + and - give the same t since only one intersection
    }
    if(discriminant > 0.0){ // Two intersections | ray hit the sphere
        

        double t1 = (-c2 + glm::sqrt(discriminant)) / (2.0 * c1);
        double t2 = (-c2 - glm::sqrt(discriminant)) / (2.0 * c1);

        if(t1 < 0.001){
            t1 = -1;
        }
        if(t2 < 0.001){
            t2 = -1;
        }

        if(t1 < DBL_EPSILON && t2 < DBL_EPSILON){
            return glm::dvec3(NAN, NAN, NAN);
        }

        if(t1 < DBL_EPSILON && t2 > DBL_EPSILON){
            return ray.rayOrigin + ray.rayDirection * t2; // If t1 is negative, t2 is positive
        }
        if(t2 < DBL_EPSILON && t1 > DBL_EPSILON){
            return ray.rayOrigin + ray.rayDirection * t1; // If t2 is negative, t1 is positive
        }

        if(t1 < t2){
            return ray.rayOrigin + ray.rayDirection * t1; // t1 is closer
        }else{
            return ray.rayOrigin + ray.rayDirection * t2; // t2 is closer
        }
    }

    return glm::dvec3(NAN, NAN, NAN); // Should never reach this point

}

glm::dvec3 Sphere::getColor()
{
    return material.getMaterialColor();
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
