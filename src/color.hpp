#include <iostream>
#include "glmlib/glm.hpp"
#include "glmlib/gtx/string_cast.hpp"

using color = glm::vec3;

void write_color(std::ostream &out, const glm::vec3 &pixel_color)
{
    auto r = pixel_color.x;
    auto g = pixel_color.y;
    auto b = pixel_color.z;

    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
