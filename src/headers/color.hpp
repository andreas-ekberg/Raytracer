#include <iostream>
#include "../glmlib/glm.hpp"
#include "../glmlib/gtx/string_cast.hpp"

using color = glm::dvec3;

double exposure = 1.0;

void write_color(std::ostream &out, const glm::dvec3 &pixel_color)
{
    double r = pixel_color.x;
    double g = pixel_color.y;
    double b = pixel_color.z;

    int rbyte = (int)(round(r * 255.0 * exposure));
    int gbyte = (int)(round(g * 255.0 * exposure));
    int bbyte = (int)(round(b * 255.0 * exposure));

    rbyte = std::clamp(rbyte, 0, 255);
    gbyte = std::clamp(gbyte, 0, 255);
    bbyte = std::clamp(bbyte, 0, 255);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
