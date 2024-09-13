#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"
#include "glmlib/glm.hpp"
#include "glmlib/gtx/string_cast.hpp"

class Camera
{

public:
    Camera(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 bottomLeft, glm::dvec3 bottomRight, glm::dvec3 eyePosition)
    {
        p[0] = topLeft;
        p[1] = topRight;
        p[2] = bottomLeft;
        p[3] = bottomRight;
        eyePos = eyePosition;
    }

    glm::dvec3 p[4];
    glm::dvec3 eyePos;

private:
protected:
};

#endif