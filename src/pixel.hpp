#ifndef PIXEL_HPP
#define PIXEL_HPP

#include "ray.hpp"
#include "glmlib/glm.hpp"
#include "glmlib/gtx/string_cast.hpp"

class Pixel
{
    public:
    Pixel();
    Pixel(glm::dvec3 pos) : position(pos) {};
    glm::dvec3 position;
    glm::dvec3 color;

    void setPos(glm::dvec3 pos){
        position = pos;
    };
};

#endif