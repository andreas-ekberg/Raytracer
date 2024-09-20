#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"
#include "pixel.hpp"
#include <vector>
#include "../glmlib/glm.hpp"
#include "../glmlib/gtx/string_cast.hpp"

class Camera
{

public:
    Camera();
    Camera(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 bottomLeft, glm::dvec3 bottomRight, glm::dvec3 eyePosition, double _pixelSizeX, double _pixelSizeY, int imageWidth, int imageHeight);

    glm::dvec3 p[4];
    glm::dvec3 eyePos;
    std::vector<Pixel> thePixels;
    double pixelSizeX;
    double pixelSizeY;
};

#endif