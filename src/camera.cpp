#ifndef CAMERA_CPP
#define CAMERA_CPP

#include "headers/camera.hpp"
Camera::Camera()
{
}
Camera::Camera(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 bottomLeft, glm::dvec3 bottomRight, glm::dvec3 eyePosition, double _pixelSizeX, double _pixelSizeY, int imageWidth, int imageHeight)
{
    p[0] = topLeft;
    p[1] = topRight;
    p[2] = bottomLeft;
    p[3] = bottomRight;
    eyePos = eyePosition;
    pixelSizeX = _pixelSizeX;
    pixelSizeY = _pixelSizeY;

    thePixels.reserve(imageHeight * imageWidth);

    for (size_t j = 0; j < imageHeight; j++)
    {
        for (size_t i = 0; i < 800; i++)
        {
            thePixels.push_back(Pixel(glm::dvec3(0.0, i * pixelSizeX - 0.99875, j * pixelSizeY - 0.99875)));
        }
    }
}

#endif