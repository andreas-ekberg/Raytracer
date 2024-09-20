#include <iostream>
#include "../glmlib/glm.hpp"
#include "../glmlib/gtx/string_cast.hpp"

using color = glm::dvec3;

void write_color(std::ostream &out, const glm::dvec3 &pixel_color)
{
    double r = pixel_color.x;
    double g = pixel_color.y;
    double b = pixel_color.z;

    int rbyte = (int)(round(r * 255.0));
    int gbyte = (int)(round(g * 255.0));
    int bbyte = (int)(round(b * 255.0));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
