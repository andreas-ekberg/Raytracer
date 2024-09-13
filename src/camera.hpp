#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"
#include "glmlib/glm.hpp"
#include "glmlib/gtx/string_cast.hpp"

class Camera
{

public:
    Camera(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 eyePosition)
    {
        p[0] = topLeft;
        p[1] = topRight;
        p[2] = bottomLeft;
        p[3] = bottomRight;
        eyePos = eyePosition;
    }

        glm::vec3 p[4];
    glm::vec3 eyePos;

private:
protected:
};

#endif