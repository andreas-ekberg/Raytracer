#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

class ray
{
public:
    ray() {}
    ray(const point3 &origin, const vec3 &direction) : rayOrigin(rayOrigin), rayDirection(rayDirection) {}

    const point3 &origin() const { return rayOrigin; }
    const vec3 &direction() const { return rayDirection; }

    point3 position(double t) const
    {
        return rayOrigin + t * rayDirection;
    }

private:
    point3 rayOrigin;
    vec3 rayDirection;
};

#endif