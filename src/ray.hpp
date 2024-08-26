#ifndef RAY_HPP
#define RAY_HPP

#include "glmlib/glm.hpp"
#include "glmlib/gtx/string_cast.hpp"

class ray
{
public:
    ray() : rayOrigin(vec3(0, 0, 0)), rayDirection(vec3(1, 0, 0)) {}
    ray(const vec3 &origin, const vec3 &direction) : rayOrigin(origin), rayDirection(direction) {}

    const glm::vec3 &origin() const { return rayOrigin; }
    const glm::vec3 &direction() const { return rayDirection; }

    glm::vec3 position(float t) const
    {
        return rayOrigin + rayDirection * t;
    }

private:
    glm::vec3 rayOrigin;
    glm::vec3 rayDirection;
};

#endif